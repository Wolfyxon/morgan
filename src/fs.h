#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "defines.h"

#ifdef TARGET_WINDOWS
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

char* freadstr(char* path);
int ftouch(char* path);
bool fexists(char* path);
int create_dir(char* path);
