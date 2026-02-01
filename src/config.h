#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "defines.h"
#include "string_util.h"

#define CONFIG_FOLDER "morgan"
#define CONFIG_FILE "morgan.conf"

typedef struct {
    char* key;
    char* value;
} ConfigEntry;

char* get_user_data_dir();
char* config_get_dir();
char* config_get_file_path();
char* config_get_section_slice(char* config_string, char* section_name);
ConfigEntry* config_get_entry(char* line);
void config_free_entry(ConfigEntry* entry);