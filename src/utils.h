#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "input.h"

void assert(bool condition, char* error);
void asset_str_eq(char* str_a, char* str_b);

bool confirm(char* message);

int key_to_note(Key key);
int offset_note(int note, int octave_offset);
