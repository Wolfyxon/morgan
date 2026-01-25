#ifndef _WIN_INPUT_H
#define _WIN_INPUT_H

#include <stdio.h>
#include <windows.h>
#include "../input.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND win_create_window();

void win_init();
void win_loop();
PRAWINPUTDEVICELIST win_get_input_devices(int* length);
Keyboard* win_get_keyboards(int* length);

#endif