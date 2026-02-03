#include <stdio.h>
#include "fs.h"
#include "defines.h"
#include "utils.h"
#include "input.h"
#include "midi.h"
#include "config.h"
#include "tests.h"

typedef struct {
    char* config_path;
    bool run_setup;
} CmdlineOptions;

int main(int argc, char** argv);
CmdlineOptions process_args(int argc, char** argv);
void setup_wizard(char* config_path, bool ask);
void print_header();
void print_help();
void start();
void main_loop();
void list_keyboards();
