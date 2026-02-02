#include <stdio.h>
#include "defines.h"
#include "utils.h"
#include "input.h"
#include "midi.h"
#include "config.h"
#include "tests.h"

typedef struct {
    char* config_path;
} CmdlineOptions;

int main(int argc, char** argv);
CmdlineOptions process_args(int argc, char** argv);
void print_header();
void print_help();
void start();
void main_loop();
void list_keyboards();
