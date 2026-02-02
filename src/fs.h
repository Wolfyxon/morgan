#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef TARGET_WINDOWS
#include <io.h>
#else
#include <unistd.h>
#endif

char* freadstr(char* path);
