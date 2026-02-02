#include "fs.h"

char* freadstr(char* path) {
    FILE* file = fopen(path, "r");
    
    if(file == NULL) {
        return NULL;
    }
    
    char* file_buf = malloc(0);
    size_t file_len = 0;

    char read_buf[64];
    size_t read_len = 0;

    if(file_buf == NULL) {
        fprintf(stderr, "error: Failed to allocate file buffer. \n");
        return NULL;
    }

    while((read_len = fread(read_buf, sizeof(char), sizeof(read_buf), file)) != 0) {
        file_len += read_len;
        file_buf = realloc(file_buf, file_len);

        if(!file_buf) {
            fprintf(stderr, "error: Failed to extend file buffer. \n");
            return NULL;
        }

        strncat(file_buf, read_buf, read_len);
    }

    fclose(file);

    return file_buf;
}
