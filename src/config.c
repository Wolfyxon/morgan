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
    buf[len] = '\0';

    return buf;
}

ConfigEntry* config_get_entry(char* string) {
    size_t len = strlen(string);
    
    size_t key_start = 0;
    size_t key_len = 0;

    for(size_t i = 0; i < len; i++) {
        if(string[i] == '=') {
            return NULL;
        }

        if(!isspace(string[i])) {
            key_start = i;
            break;
        }

        key_start++;
    }

    for(size_t i = 0; i < len; i++) {
        if(string[key_start + i] == '\n') {
            return NULL;
        }
        
        if(string[key_start + i] == '=') {
            key_len = i;
            break;
        }
    }

    size_t value_start = key_start + key_len + 1;
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
        if(string[i] == '\n') {
            break;
        }

        value_len++;
    }
    
    char key_buf[key_len + 1];
    char value_buf[value_len + 1];

    strncpy(key_buf, string + key_start, key_len);
    strncpy(value_buf, string + value_start, value_len);
    
    key_buf[key_len] = '\0';
    value_buf[value_len] = '\0';
    
    ConfigEntry* entry = malloc(sizeof(ConfigEntry));

    if(entry == NULL) {
        fprintf(stderr, "error: Failed to allocate config entry buffer \n");
        exit(1);
    }

    entry->key = strtrim(key_buf);
    entry->value = strtrim(value_buf);

    return entry;
}

void config_free_entry(ConfigEntry* entry) {
    free(entry->key);
    free(entry->value);
    free(entry);
}

char* config_get_value(char* config_string, char* section, char* key) {
    char* section_slice = config_get_section_slice(config_string, section);

    char* line = strtok(section_slice, "\n");

    while(line != NULL) {
        ConfigEntry* entry = config_get_entry(line);
        
        if(entry == NULL) {
            continue;
        }

        if(strcmp(key, entry->key) == 0) {
            char* res = malloc(strlen(entry->value) + 1);
            
            strcpy(res, entry->value);
            config_free_entry(entry);
            free(section_slice);

            return res;
        }

        config_free_entry(entry);
        line = strtok(NULL, "\n");
    }

    free(section_slice);
    return NULL;
}

char** config_get_sections(char* config_string, size_t* len_ptr) {
    char* line = strtok(config_string, "\n");
    char** res = checked_malloc(0, "config section list");

    while(line != NULL) {
        size_t line_len = strlen(line);

        if(line_len > 2) {
            char first_char = line[0];
            char last_char = line[line_len - 1];

            if(first_char == '[' && last_char == ']') {
                char* section = checked_malloc(line_len - 1, "section name buffer");
                strncpy(section, line + 1, line_len - 1);

                checked_realloc(res, *len_ptr++, "config section list");
                res[*len_ptr] = section;
            }
        }

        line = strtok(NULL, "\n");
    }

    return res;
}

void config_free_section_list(char** sections, size_t len) {
    for(size_t i = 0; i < len; i++) {
        free(sections[i]);
    }

    free(sections);
}

KeyboardConfig* config_get_keyboards(char* config_string, size_t* len_ptr) {
    size_t sections_len = 0;
    char** sections = config_get_sections(config_string, &sections_len);
    
    config_free_section_list(sections, sections_len);
}
