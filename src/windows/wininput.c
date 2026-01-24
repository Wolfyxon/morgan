#include "wininput.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_INPUT) {
        UINT size;
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));

        BYTE buf[size];
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buf, &size, sizeof(RAWINPUTHEADER));

        RAWINPUT *raw = (RAWINPUT*)buf;

        if (raw->header.dwType == RIM_TYPEKEYBOARD) {
            printf("Key %u from device %p\n", raw->data.keyboard.VKey, raw->header.hDevice);
        }
    }
    
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

HWND win_create_window(HINSTANCE hInstance) {
    HINSTANCE hdInstance = GetModuleHandle(NULL);

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "RawInputHiddenWindow";

    RegisterClass(&wc);

    return CreateWindowEx(
        0,
        wc.lpszClassName,
        NULL,
        0,
        0, 0, 0, 0,
        HWND_MESSAGE,
        NULL,
        hInstance,
        NULL
    );
}

void win_loop() {
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        printf("message \n");
    }
}

void win_init() {
    HWND window = win_create_window();

    if(!window) {
        fprintf(stderr, "Failed to create window.\n");
        exit(1);
    }

    RAWINPUTDEVICE device = {0};
    device.usUsagePage = 0x01;
    device.usUsage = 0x06; // keyboard
    device.dwFlags = RIDEV_INPUTSINK;
    device.hwndTarget = window;

    if (!RegisterRawInputDevices(&device, 1, sizeof(device))) {
        printf("Failed to register input device\n");
        exit(1);
    }

    win_loop();
}
