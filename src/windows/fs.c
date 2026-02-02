#include <windows.h>

int mkdir(char* path, unsigned int mode) {
    return !CreateDirectory(path, NULL);
}

int rmdir(char* path) {
    return !RemoveDirectory(path);
}
