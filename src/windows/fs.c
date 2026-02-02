#include <windows.h>

int mkdir(char* path, unsigned int mode) {
    return CreateDirectory(path, NULL);
}
