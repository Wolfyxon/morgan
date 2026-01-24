#ifndef _WIN_INPUT_H
#define _WIN_INPUT_H

#include <stdio.h>
#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND win_create_window(HINSTANCE hInstance);

void win_init();
void win_loop();

#endif