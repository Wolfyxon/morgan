#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "string_util.h"

#define CONFIG_FOLDER "morgan"
#define CONFIG_FILE "morgan.conf"

char* get_user_data_dir();
char* config_get_dir();
char* config_get_file_path();
