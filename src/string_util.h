#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

size_t strcountchar(char* string, char char_to_count);
size_t strcountlines(char* string);
int strfind(char* string, char* search);
char* strremovechar(char* string, char char_to_remove);
char* strtolf(char* string);
