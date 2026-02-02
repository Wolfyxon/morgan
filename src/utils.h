#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "input.h"

#ifdef TARGET_WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

void assert(bool condition, char* error);
void asset_str_eq(char* str_a, char* str_b);

bool confirm(char* message);

void sleep_ms(unsigned long ms);

void* checked_malloc(size_t size, char* buffer_name);
void* checked_realloc(void* buffer, size_t size, char* buffer_name);

int key_to_note(Key key);
int offset_note(int note, int octave_offset);
