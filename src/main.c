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

    if(cmdline_options.run_setup || !fexists(config_path)) {
        setup_wizard(config_path, !cmdline_options.run_setup);

        if(cmdline_options.run_setup) {
            return 0;
        }
    }

    char* config_str = config_read_file(config_path);
        
    size_t keyboards_len = 0;
    KeyboardData* keyboards = prepare_keyboards_from_config(config_str, &keyboards_len);

    assert(keyboards != NULL, "prepare_keyboards_from_config() returned NULL");

    start(keyboards, keyboards_len);
    
    return 0;
}

KeyboardData* prepare_keyboards_from_config(char* config_string, size_t* len_ptr) {
    if(config_string == NULL) {
        return NULL;
    }

    size_t keyboard_configs_len = 0;
    KeyboardConfig* keyboard_configs = config_get_keyboards(config_string, &keyboard_configs_len);

    KeyboardData* keyboards = checked_malloc(keyboard_configs_len * sizeof(KeyboardData), "keyboard data list");

    for(size_t i = 0; i < keyboard_configs_len; i++) {
        KeyboardConfig conf = keyboard_configs[i];
        
        KeyboardData kb = {
            .id = conf.id,
            .octave_offset = conf.octave,
            .pressed_keys = {KEY_NONE},
            .pressed_keys_len = 0
        };

        keyboards[i] = kb;
    }

    *len_ptr = keyboard_configs_len;
    free(keyboard_configs);

    return keyboards;
}

CmdlineOptions process_args(int argc, char** argv) {
    CmdlineOptions res = {0};

    for(size_t i = 1; i < argc; i++) {
        char* arg = argv[i];
        
        if(is_flag(arg, "help")) {
            print_header();
            print_help();
            exit(0);
        }

        if(is_flag(arg, "config")) {
            if(i + 1 < argc) {
                res.config_path = argv[i + 1];
                i++;
                continue;
            } else {
                fprintf(stderr, "error: %s expects a value \n", arg);
                exit(1);
            }
        }

        if(is_flag(arg, "keyboards")) {
            input_init();
            list_keyboards();
            exit(0);
        }

        if(is_flag(arg, "setup")) {
            res.run_setup = true;
            continue;
        }

        fprintf(stderr, "error: Unrecognized argument '%s'\n", arg);
        print_help();
        exit(1);
    }
    
    return res;
}

void setup_wizard(char* config_path, bool ask) {
    char* config_dir = config_get_dir();

    if(ask && !confirm("Would you like to run the configuration wizard?")) {
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

    if(create_dir(config_dir) != 0) {
        fprintf(stderr, "error: Failed to create directory '%s' \n", config_dir);
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
        printf("Press any key on a desired keyboard...\n");
        fflush(stdout);

        input_flush();
        sleep_ms(100);
        input_flush();

        KeyEvent key_ev;
        while((key_ev = input_poll_event()).key == KEY_NONE);

        printf("Keyboard id: %d \n", key_ev.device);
        int octave = input_int("Enter octave number");
        
        char kb_config[1024] = {0};
        snprintf(kb_config, sizeof(kb_config), "[keyboard_%d]\noctave = %d\n\n", key_ev.device, octave);
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
    printf("Usage: morgan [-h | --help] [-c <path> | --config <path>] \n");
    printf("              [-k | --keyboards] | [-s | --setup] \n");
    printf("\n");
    
    printf("Options: \n");
    printf("    -h, --help        Displays help. \n");
    printf("    -c, --config      Specifies the config file path. \n");
    printf("    -k, --keyboards   Lists detected keyboards.\n");
    printf("    -s, --setup       Runs the configuration wizard and exits.\n");
}

void start(KeyboardData* keyboards, size_t keyboards_len) {
    printf("Starting...\n");
    
    input_init();
    midi_init();
    
    printf("Ready!\n");
    printf("Select the \"Morgan MIDI\" input port in your DAW.\n");
    
    input_flush();
    main_loop(keyboards, keyboards_len);
}

void main_loop(KeyboardData* keyboards, size_t keyboards_len) {
     while(1) {
        KeyEvent key_ev = input_poll_event();

        if(key_ev.key == KEY_NONE || key_ev.key == KEY_UNKNOWN) {
            continue;
        }

        if(get_keyboard_data(key_ev.device, keyboards, keyboards_len) == NULL) {
            printf("Keyboard %d is not in the config!\n", key_ev.device);
            add_unconfigured_keyboard(key_ev.device, &keyboards, &keyboards_len);
        }

        KeyboardData* kb = get_keyboard_data(key_ev.device, keyboards, keyboards_len);

        if(key_ev.pressed && is_key_pressed(kb, key_ev.key)) {
            continue;
        }

        printf("Key %d %d from %d \n", key_ev.key, key_ev.pressed, key_ev.device);
        set_key_pressed(kb, key_ev.key, key_ev.pressed);
        
        int note = offset_note(key_to_note(key_ev.key), kb->octave_offset);

        if(note == INVALID_NOTE) {
            continue;
        }

        if(key_ev.pressed) {
            midi_note_on(note, 50);
        } else {
            midi_note_off(note, 50);
        }
    }
}

void set_key_pressed(KeyboardData* keyboard, Key key, bool pressed) {
    // TODO: Do stuff with keyboard->pressed_keys_len to decrease iteration count
    for(size_t i = 0; i < KEYBOARD_MAX_PRESSED_KEYS; i++) {
        printf("%d ", keyboard->pressed_keys[i]);
        if(pressed && keyboard->pressed_keys[i] == KEY_NONE) {
            keyboard->pressed_keys[i] = key;
            return;
        }

        if(!pressed && keyboard->pressed_keys[i] == key ) {
            keyboard->pressed_keys[i] = KEY_NONE;
            return;
        }
    }
}

bool is_key_pressed(KeyboardData* keyboard, Key key) {
    for(size_t i = 0; i < KEYBOARD_MAX_PRESSED_KEYS; i++) {
        if(keyboard->pressed_keys[i] == key) {
            return true;
        }
    }

    return false;
}

void add_unconfigured_keyboard(int id, KeyboardData** array_ptr, size_t* len_ptr) {
    KeyboardData kb = {
        .id = id,
        .octave_offset = DEFAULT_OCTAVE,
        .pressed_keys = {KEY_NONE},
        .pressed_keys_len = 0
    };

    *len_ptr += 1;
    *array_ptr = checked_realloc(*array_ptr, sizeof(KeyboardData) * (*len_ptr), "keyboard list");
    (*array_ptr)[*len_ptr - 1] = kb;
}

KeyboardData* get_keyboard_data(int id, KeyboardData* keyboards, size_t len) {
    for(size_t i = 0; i < len; i++) {
        if(keyboards[i].id == id) {
            return &keyboards[i];
        }
    }

    return NULL;
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