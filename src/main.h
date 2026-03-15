#include <stdio.h>
#include "fs.h"
#include "defines.h"
#include "utils.h"
#include "input.h"
#include "midi.h"
#include "config.h"
#include "tests.h"

#define KEYBOARD_MAX_PRESSED_KEYS 32

typedef struct {
    char* config_path;
    bool run_setup;
} CmdlineOptions;

typedef struct {
    int id;
    int octave_offset;
    Key pressed_keys[KEYBOARD_MAX_PRESSED_KEYS];
    size_t pressed_keys_len;
} KeyboardData;

int main(int argc, char** argv);
void print_keyboards(KeyboardData* keyboards, size_t len);
KeyboardData* prepare_keyboards_from_config(char* config_string, size_t* len_ptr);
CmdlineOptions process_args(int argc, char** argv);
void setup_wizard(char* config_path, bool ask);
void print_header();
void print_help();
void start(KeyboardData* keyboards, size_t keyboards_len);
void main_loop(KeyboardData* keyboards, size_t keyboards_len);
void set_key_pressed(KeyboardData* keyboard, Key key, bool pressed);
bool is_key_pressed(KeyboardData* keyboard, Key key);
void add_unconfigured_keyboard(int id, KeyboardData** array_ptr, size_t* len_ptr);
KeyboardData* get_keyboard_data(int id, KeyboardData* keyboards, size_t len);
void list_keyboards();
