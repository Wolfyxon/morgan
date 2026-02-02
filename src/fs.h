#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifdef TARGET_WINDOWS
#include "windows/fs.c"
#include <io.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

#define MKDIR_MODE 0777

char* freadstr(char* path);
bool fexists(char* path);
