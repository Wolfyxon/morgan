#include "main.h"

int main(int argc, char** argv) {
    run_tests();

    CmdlineOptions cmdline_options = process_args(argc, argv);
    char* config_path;

    if(cmdline_options.config_path != NULL) {
        if(!fexists(cmdline_options.config_path)) {
            fprintf(stderr, "error: Config file '%s' not found\n", cmdline_options.config_path);
            exit(1);
        }

        config_path = cmdline_options.config_path;
    } else {
        config_path = config_get_file_path();
    }

    print_header();

    if(!fexists(config_path)) {
        setup_wizard(config_path);
    }

    start();
    
    return 0;
}

CmdlineOptions process_args(int argc, char** argv) {
    CmdlineOptions res = {0};

    for(size_t i = 1; i < argc; i++) {
        char* arg = argv[i];
        
        if(strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
            print_header();
            print_help();
            exit(0);
        }

        if(strcmp(arg, "--config") == 0 || strcmp(arg, "-c") == 0) {
            if(i + 1 < argc) {
                res.config_path = argv[i + 1];
                i++;
                continue;
            } else {
                fprintf(stderr, "error: %s expects a value \n", arg);
                exit(1);
            }
        }

        if(strcmp(arg, "--keyboards") == 0 || strcmp(arg, "-k") == 0) {
            input_init();
            list_keyboards();
            exit(0);
        }

        fprintf(stderr, "error: Unrecognized argument '%s'\n", arg);
        print_help();
        exit(1);
    }
    
    return res;
}

void setup_wizard(char* config_path) {
    char* config_dir = config_get_dir();

    if(!confirm("Would you like to run the configuration wizard?")) {
        if(create_dir(config_dir) != 0) {
            fprintf(stderr, "error: Failed to create directory '%s'. Program will continue in default state. \n", config_dir);
            return;
        }
        return;

        if(ftouch(config_path) != 0) {
            fprintf(stderr, "error: Failed to create file '%s'. Program will continue in a default state\n", config_path);
        }

        printf("Empty config file was created at: %s \n", config_path);
        printf("Program will continue in a default state \n");

        return;
    }

    FILE* file = fopen(config_path, "w");

    if(file == NULL) {
        fprintf(stderr, "error: Failed to open '%s' for writing. \n", config_path);
        exit(1);
    }

    char* config_string = checked_malloc(0, "config string");
    size_t config_len = 0;

    input_init();

    while(1) {
        input_poll_event();

        printf("Press any key on a desired keyboard...\n");
        fflush(stdout);

        KeyEvent key_ev;
        while((key_ev = input_poll_event()).key == KEY_NONE);

        printf("Keyboard id: %d \n", key_ev.device);
        int octave = input_int("Enter octave number");
        
        char kb_config[1024] = {0};
        snprintf(kb_config, sizeof(kb_config), "[keyboard_%d]\noctave = %d", key_ev.device, octave);
        size_t len = strlen(kb_config);

        size_t old_len = config_len;
        config_len += len + 1;
        config_string = checked_realloc(config_string, config_len, "config string");

        memset(config_string + old_len, '\0', config_len - old_len);
        strcat(config_string, kb_config);

        if(!confirm("Add another keyboard?")) {
            break;
        }
    }

    fprintf(file, "%s", config_string);
    fclose(file);
    free(config_string);

    printf("Configuration saved in '%s' \n", config_path);
}

void print_header() {
    printf("Morgan v%s by Wolfyxon\n", VERSION);
    printf("https://github.com/Wolfyxon/morgan\n\n");
}

void print_help() {
    printf("Usage: morgan [-h | --help] [--config <path> | -c <path>] \n\n");
    
    printf("Options: \n");
    printf("    -h, --help        Displays help. \n");
    printf("    -c, --config      Specifies the config file path. \n");
    printf("    -k, --keyboards   Lists detected keyboards\n");
}

void start() {
    printf("Starting...\n");
    
    input_init();
    midi_init();
    
    printf("Ready!\n");
    printf("Select the \"Morgan MIDI\" input port in your DAW.\n");
    
    main_loop();
}

void main_loop() {
     while(1) {
        KeyEvent key_ev = input_poll_event();

        if(key_ev.key == KEY_NONE || key_ev.key == KEY_UNKNOWN) {
            continue;
        }

        printf("Key %d %d from %d \n", key_ev.key, key_ev.pressed, key_ev.device);

        int note = key_to_note(key_ev.key);

        if(note == 0) {
            continue;
        }

        if(key_ev.pressed) {
            midi_note_on(note, 50);
        } else {
            midi_note_off(note, 50);
        }
    }
}

void list_keyboards() {
    int length = 0;
    Keyboard* keyboards = input_get_keyboards(&length);

    printf("Detected keyboards: \n");

    for(int i = 0; i < length; i++) {
        Keyboard kb = keyboards[i];

        printf("  %d: %s \n", kb.id, kb.name);
    }

    free(keyboards);
}