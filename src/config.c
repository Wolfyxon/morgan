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

ConfigEntry* config_get_entry(char* string) {
    size_t len = strlen(string);
    
    size_t key_start = 0;
    size_t key_len = 0;

    for(size_t i = 0; i < len; i++) {
        if(string[i] == '\n' || string[i] == '=') {
            return NULL;
        }

        if(!isspace(string[i])) {
            break;
        }

        key_start++;
    }

    for(size_t i = key_start; i < len; i++) {
        key_len++;

        if(string[i] == '\n') {
            return NULL;
        }

        if(string[i] == '=') {
            break;
        }
    }

    size_t value_start = key_start + key_len + 2;
    size_t value_len = 0;

    if(value_start >= len) {
        return NULL;
    }

    for(size_t i = value_start; i < len; i++) {
        if(string[i] == '\n') {
            return NULL;
        }
        
        if(!isspace(string[i])) {
            break;
        }

        value_start++;
    }

    for(size_t i = value_start; i < len; i++) {
        value_len++;
        
        if(string[i] == '\n') {
            break;
        }
    }
    
    char* key_buf = malloc(key_len + 1);
    char* value_buf = malloc(value_len + 1);
    
    if(key_buf == NULL) {
        fprintf(stderr, "error: Failed to allocate key string buffer \n");
        exit(1);
    }

    if(value_buf == NULL) {
        fprintf(stderr, "error: Failed to allocate value string buffer \n");
        exit(1);
    }

    strncpy(key_buf, string + key_start, key_len);
    strncpy(value_buf, string + value_start, value_len);

    ConfigEntry* entry = malloc(sizeof(ConfigEntry));

    entry->key = key_buf;
    entry->value = value_buf;

    return entry;
}

void config_free_entry(ConfigEntry* entry) {
    free(entry->key);
    free(entry->value);
    free(entry);
}
