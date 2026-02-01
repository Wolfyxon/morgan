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

    return len;
}

char* strtolf(char* string) {
    return strremovechar(string, '\r');
}
