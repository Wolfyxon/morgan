#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifdef TARGET_WINDOWS
#include <io.h>
#else
#include <unistd.h>
#endif

char* freadstr(char* path);
bool fexists(char* path);
