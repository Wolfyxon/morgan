#include "config.h"

char* get_user_data_dir() {
    #if defined(TARGET_WINDOWS)
        char* src = getenv("APPDATA");

        if(strlen(src) == 0) {
            fprintf(stderr, "error: Unable to get user config directory. %%APPDATA%% is not set.");
            exit(1);
        }

        char* buf = malloc(strlen(src) + 1);

        if(buf == NULL) {
            fprintf(stderr, "error: Unable to allocate path buffer.");
            exit(1);
        }

        strcpy(buf, src);

        return buf;
    #elif defined(TARGET_LINUX)
        char* home = getenv("HOME");
        char* suffix = "/.config";

        if(strlen(home) == 0) {
            fprintf(stderr, "error: Unable to get user config directory. HOME is not set.");
            exit(1);
        }

        char* buf = malloc(strlen(home) + strlen(suffix) + 1);

        if(buf == NULL) {
            fprintf(stderr, "error: Unable to allocate path buffer.");
            exit(1);
        }

        sprintf(buf, "%s%s", home, suffix);

        return buf;
    #endif
}

char* config_get_dir() {
    char* user_dir = get_user_data_dir();
    char* buf = malloc(strlen(user_dir) + strlen(CONFIG_FOLDER) + 2);
    
    sprintf(buf, "%s/%s", user_dir, CONFIG_FOLDER);
    free(user_dir);

    return buf;
}

char* config_get_file_path() {
    char* dir = config_get_dir();
    char* buf = malloc(strlen(dir) + strlen(CONFIG_FILE) + 2);

    sprintf(buf, "%s/%s", dir, CONFIG_FILE);
    free(dir);

    return buf;
}

char* config_process_string(char* config_string) {
    return strtolf(config_string);
}

char* config_get_section_slice(char* config_string, char* section_name) {
    size_t header_len = strlen(section_name) + 2;
    char header_str[header_len + 1];
    sprintf(header_str, "[%s]", section_name);

    char* after = strstr(config_string, header_str);
    size_t after_len = strlen(after);

    if(after_len < header_len) {
        return NULL;
    }

    char* buf = malloc(0);
    size_t len = 0;

    if(buf == NULL) {
        fprintf(stderr, "error: Failed to allocate config slice buffer \n");
        exit(1);
    }

    for(size_t i = header_len; i < after_len; i++) {
        if(after[i] != '[') {
            len++;
        } else {
            break;
        }
    }

    buf = realloc(buf, len + 1);

    if(buf == NULL) {
        fprintf(stderr, "error: Failed to reallocate config slice buffer \n");
        exit(1);
    }

    strncpy(buf, after + header_len, len);

    return buf;
}