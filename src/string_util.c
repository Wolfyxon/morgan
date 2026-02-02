#include "string_util.h"

size_t strcountchar(char* string, char char_to_count) {
    size_t len = strlen(string);
    size_t res = 0;

    for(size_t i = 0; i < len; i++) {
        if(string[i] == char_to_count) {
            res += 1;
        }
    }

    return res;
}

size_t strcountlines(char* string) {
    return strcountchar(string, '\n');
}

int strfind(char* string, char* search) {
    size_t len = strlen(string);
    size_t search_len = strlen(search);

    for(int i = 0; i < len; i++) {
        bool matches = true;

        for(int j = 0; j < search_len; j++) {
            int idx = i + j;

            if(idx >= len) {
                matches = false;
                break;
            }

            if(string[idx] != search[j]) {
                matches = false;
                break;
            }
        }

        if(matches) {
            return i;
        }
    }

    return -1;
}

bool strisnum(char* string) {
    size_t len = strlen(string);

    if(len == 0) {
        return false;
    }

    for(size_t i = 0; i < len; i++) {
        if(!isdigit(string[i])) {
            return false;
        }
    }

    return true;
}

char* strremovechar(char* string, char char_to_remove) {
    size_t len = strlen(string);
    size_t new_len = 0;

    char* buf = malloc(len + 1);

    if(buf == NULL) {
        fprintf(stderr, "error: Failed to allocate string buffer");
        exit(1);
    }

    for(size_t i = 0; i < len; i++) {
        if(string[i] != char_to_remove) {
            buf[i] = string[i];
            new_len += 1;
        }
    }

    buf = realloc(buf, new_len + 1);

    if(buf == NULL) {
        fprintf(stderr, "error: Failed to shrink string buffer");
        exit(1);
    }

    return buf;
}

char* strtolf(char* string) {
    return strremovechar(string, '\r');
}

char* strtrim(char* string) {
    size_t len = strlen(string);

    size_t start = 0;
    size_t end = 0;

    for(size_t i = 0; i < len; i++) {
        if(!isspace(string[i])) {
            start = i;
            break;
        }
    }

    for(size_t i = len - 1; i >= 0; i--) {
        if(!isspace(string[i])) {
            end = i + 1;
            break;
        }
    }

    size_t new_len = end - start;
    char* buf = malloc(new_len + 1);

    if(buf == NULL) {
        fprintf(stderr, "error: Failed to allocate string buffer");
        exit(1);
    }

    strncpy(buf, string + start, new_len);
    buf[new_len] = '\0';

    return buf;
}
