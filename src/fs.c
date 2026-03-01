#include "fs.h"

char* freadstr(char* path) {
    FILE* file = fopen(path, "r");
    
    if(file == NULL) {
        return NULL;
    }
    
    char* file_buf = malloc(0);
    size_t file_len = 0;

    char read_buf[64] = {0};
    size_t read_len = 0;

    if(file_buf == NULL) {
        fprintf(stderr, "error: Failed to allocate file buffer. \n");
        fclose(file);
        
        return NULL;
    }

    while((read_len = fread(read_buf, 1, sizeof(read_buf), file)) > 0) {
        file_buf = realloc(file_buf, file_len + 1);

        if(!file_buf) {
            fprintf(stderr, "error: Failed to extend file buffer. \n");

            free(file_buf);
            fclose(file);

            return NULL;
        }

        memcpy(file_buf + file_len, read_buf, read_len);
        file_len += read_len;
    }

    fclose(file);

    if(file_len != 0) {
        file_buf[file_len - 1] = '\0';
    }
    
    return file_buf;
}

int ftouch(char* path) {
    FILE* file = fopen(path, "w");
    
    if(file == NULL) {
        return 1;
    }

    fclose(file);
    return 0;
}

bool fexists(char* path) {
    return access(path, 0) == 0;
}

int create_dir(char* path) {
    if(fexists(path)) {
        return 0;
    }

    #ifdef TARGET_WINDOWS
    return !CreateDirectory(path, NULL);
    #else
    return mkdir(path, 0777);
    #endif
}
