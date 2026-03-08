#include "utils.h"

void assert(bool condition, char* error) {
    if(!condition) {
        fprintf(stderr, "error: %s\n", error);
        exit(1);
    }
}

void assert_str_eq(char* str_a, char* str_b) {
    if(strcmp(str_a, str_b) != 0) {
        fprintf(stderr, "error: Strings not equal\n");
        fprintf(stderr, "-- Left -- \n'%s'\n", str_a);
        fprintf(stderr, "-- Right -- \n'%s'\n", str_b);
        
        exit(1);
    }
}

void flush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int input_int(char* message) {
    char buf[256];

    while(1) {
        printf("%s: ", message);
        scanf("%255s", buf);

        if(strisnum(buf)) {
            return atoi(buf);
        }

        fprintf(stderr, "error: Please enter a valid number.\n");
    }
}

bool is_flag(char* arg, char* flag) {
    size_t arg_len = strlen(arg);
    size_t flag_len = strlen(flag);
    
    // --flag
    if(arg_len == 2) {
        char flag_short_prefixed[3];
        sprintf(flag_short_prefixed, "-%c", *flag);

        return streq(arg, flag_short_prefixed);
    }
    // -f
    else {
        char flag_prefixed[flag_len + 3];
        sprintf(flag_prefixed, "--%s", flag);

        return streq(arg, flag_prefixed);
    }
}

bool confirm(char* message) {
    printf("%s [Y/N]: ", message);
    fflush(stdout);
    
    char inp[3] = {0};
    scanf("%3s", inp);

    flush_stdin();

    return inp[0] == 'y' || inp[0] == 'Y';
}

void sleep_ms(unsigned long ms) {
    #ifdef TARGET_WINDOWS
    Sleep(ms);
    #else
    usleep(ms * 1000);
    #endif
}

void* checked_malloc(size_t size, char* buffer_name) {
    void* buf = malloc(size);

    if(buf == NULL) {
        fprintf(stderr, "error: Failed to allocate %s\n", buffer_name);
        exit(1);
    }

    return buf;
}

void* checked_realloc(void* buffer, size_t size, char* buffer_name) {
    void* buf = realloc(buffer, size);

    if(buf == NULL) {
        fprintf(stderr, "error: Failed to reallocate %s\n", buffer_name);
        exit(1);
    }

    return buf;
}

int key_to_note(Key key) {
    switch(key) {
        // TODO: Add more notes 
        
        case KEY_Q: return 60; // C4
        case KEY_2: return 61; // C#4
        case KEY_W: return 62; // D4
        case KEY_3: return 63; // D#4
        case KEY_E: return 64; // E4
        case KEY_R: return 65; // F4
        case KEY_5: return 66; // F#4
        case KEY_T: return 67; // G4
        case KEY_6: return 68; // G#4
        case KEY_Y: return 69; // A4
        case KEY_7: return 70; // A#4
        case KEY_U: return 71; // B4

        case KEY_I: return 72; // C5
        case KEY_9: return 73; // C#5
        case KEY_O: return 74; // D5
        case KEY_0: return 75; // D#5
        case KEY_P: return 76; // E5
        case KEY_L_BRACKET: return 77; // F5
        case KEY_EQUALS: return 78; // F#5
        case KEY_R_BRACKET: return 79; // G5

        default: return INVALID_NOTE;
    }
}

int offset_note(int note, int octave_offset) {
    return note + (12 * octave_offset);
}
