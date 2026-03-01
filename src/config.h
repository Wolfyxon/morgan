#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "defines.h"
#include "utils.h"
#include "string_util.h"

#define CONFIG_FOLDER "morgan"
#define CONFIG_FILE "morgan.conf"

#define KEYBOARD_SECTION_PREFIX "keyboard_" 

typedef struct {
    char* key;
    char* value;
} ConfigEntry;

typedef struct {
    int id;
    int octave;
} KeyboardConfig;

char* get_user_data_dir();
char* config_get_dir();
char* config_get_file_path();
char* config_get_section_slice(char* config_string, char* section_name);
ConfigEntry* config_get_entry(char* line);
void config_free_entry(ConfigEntry* entry);
char* config_get_value(char* config_string, char* section, char* key);
char** config_get_sections(char* config_string, size_t* len_ptr);
void config_free_section_list(char** sections, size_t len);
KeyboardConfig* config_get_keyboards(char* config_string, size_t* len_ptr);
