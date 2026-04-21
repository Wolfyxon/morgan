#include "wininput.h"

#define MAX_KEY_EVENTS 1024

KeyEvent key_event_queue[MAX_KEY_EVENTS];
size_t key_event_queue_start = 0;
size_t key_event_queue_end = 0;

const KeyEvent KEY_EVENT_NONE = {
    .key = KEY_NONE,
    .device = 0,
    .pressed = false
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_INPUT) {
        UINT size;
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));

        BYTE buf[size];
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buf, &size, sizeof(RAWINPUTHEADER));

        RAWINPUT *raw = (RAWINPUT*)buf;

        if (raw->header.dwType == RIM_TYPEKEYBOARD) {
            #ifdef INPUT_DEBUG

            printf(
                "[windows] Key %u from device %p flags: %u msg: %x\n", 
                raw->data.keyboard.VKey, 
                raw->header.hDevice, 
                raw->data.keyboard.Flags,
                raw->data.keyboard.Message
            );

            #endif

            KeyEvent event = {
                .key = win_input_to_key(raw->data.keyboard.VKey),
                .device = (int)raw->header.hDevice,
                .pressed = !(raw->data.keyboard.Flags & RI_KEY_BREAK)
            };

            win_push_key_event(event);
        }
    }
    
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

HWND win_create_window() {
    HINSTANCE h_instance = GetModuleHandle(NULL);

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = h_instance;
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
        h_instance,
        NULL
    );
}

void win_push_key_event(KeyEvent event) {
    int next = (key_event_queue_start + 1) % MAX_KEY_EVENTS;

    if(next == key_event_queue_end) {
        fprintf(stderr, "warning: Input queue full, event dropped");
        return;
    }

    key_event_queue[next] = event;
    key_event_queue_start = next;
}

void win_flush_events() {
    MSG msg;
    
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

KeyEvent win_poll_key_event() {
    win_flush_events();
    
    if(key_event_queue_start == key_event_queue_end) {
        return KEY_EVENT_NONE;
    }

    KeyEvent event = key_event_queue[key_event_queue_end];
    key_event_queue_end = (key_event_queue_end + 1) % MAX_KEY_EVENTS;

    return event;
}

void win_flush_input() {
    while(key_event_queue_start != key_event_queue_end) {
        win_poll_key_event();
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
}

PRAWINPUTDEVICELIST win_get_input_devices(int* length) {
    UINT device_count;
    PRAWINPUTDEVICELIST device_list = NULL;

    // totally not stolen from michaelsoft binbows documentation
    while(1) {
        if(GetRawInputDeviceList(NULL, &device_count, sizeof(RAWINPUTDEVICELIST)) != 0) { 
            fprintf(stderr, "error: Failed to get list of devices.");
            return NULL;
        }

        if(device_count == 0) { 
            break; 
        }

        if((device_list = malloc(sizeof(RAWINPUTDEVICELIST) * device_count)) == NULL) {
            fprintf(stderr, "error: Failed to allocate device list.");
            return NULL;
        }

        device_count = GetRawInputDeviceList(device_list, &device_count, sizeof(RAWINPUTDEVICELIST));

        if(device_count == (UINT)-1) {
            if(GetLastError() != ERROR_INSUFFICIENT_BUFFER) { 
                fprintf(stderr, "error: Insufficient buffer.");
                return NULL;
            }
            
            free(device_list);
            continue;
        }
        break;
    }

    *length = device_count;
    return device_list;
}

Keyboard* win_get_keyboards(int* length) {
    int device_count = 0;
    PRAWINPUTDEVICELIST device_list = win_get_input_devices(&device_count);

    if(device_list == NULL) {
        fprintf(stderr, "error: Unable to get input devices. \n");
        return NULL;
    }

    if(device_count == 0) {
        fprintf(stderr, "warning: No keyboards found. This is probably a bug. \n");
    }

    Keyboard* keyboards = malloc(0);
    int keyboard_count = 0;

    for(int i = 0; i < device_count; i++) {
        RAWINPUTDEVICELIST device = device_list[i];
        
        char* name_buf;
        UINT name_len;

        // TODO: Human readable name
        if(device.dwType == RIM_TYPEKEYBOARD) {
            if(GetRawInputDeviceInfoA(device.hDevice, RIDI_DEVICENAME, NULL, &name_len) == (UINT)-1) {
                fprintf(stderr, "error: Skipping device %p. Failed to get name length.", device.hDevice);
                continue;
            }

            if(name_len == 0) {
                continue;
            }

            name_buf = malloc(name_len);

            if(!name_buf) {
                fprintf(stderr, "error: Skipping device %p. Failed to allocate name buffer.", device.hDevice);
                continue;
            }

            if(GetRawInputDeviceInfoA(device.hDevice, RIDI_DEVICENAME, name_buf, &name_len) == (UINT)-1) {
                fprintf(stderr, "error: Skipping device %p. Failed to get name. \n", device.hDevice);
                continue;
            }
            
            Keyboard kb = {
                .id = (int) device.hDevice, // TODO: Fix: warning: cast from pointer to integer of different size
                .name = name_buf
            };

            keyboard_count += 1;
            keyboards = realloc(keyboards, sizeof(Keyboard) * keyboard_count);

            if(keyboards == NULL) {
                fprintf(stderr, "error: Failed to extend keyboard list. \n");
                return NULL;
            }

            keyboards[keyboard_count - 1] = kb;
        }
    }

    free(device_list);
    *length = keyboard_count;
    return keyboards;
}

Key win_input_to_key(int code) {
	switch(code) {
		case 65: return KEY_A;
		case 66: return KEY_B;
		case 67: return KEY_C;
		case 68: return KEY_D;
		case 69: return KEY_E;
		case 70: return KEY_F;
		case 71: return KEY_G;
		case 72: return KEY_H;
		case 73: return KEY_I;
		case 74: return KEY_J;
		case 75: return KEY_K;
		case 76: return KEY_L;
		case 77: return KEY_M;
		case 78: return KEY_N;
		case 79: return KEY_O;
		case 80: return KEY_P;
		case 81: return KEY_Q;
		case 82: return KEY_R;
		case 83: return KEY_S;
		case 84: return KEY_T;
		case 85: return KEY_U;
		case 86: return KEY_V;
		case 87: return KEY_W;
		case 88: return KEY_X;
		case 89: return KEY_Y;
		case 90: return KEY_Z;
		
		case 48: return KEY_0;
		case 49: return KEY_1;
		case 50: return KEY_2;
		case 51: return KEY_3;
		case 52: return KEY_4;
		case 53: return KEY_5;
		case 54: return KEY_6;
		case 55: return KEY_7;
		case 56: return KEY_8;
		case 57: return KEY_9;
		
		case 189: return KEY_MINUS;
		case 187: return KEY_EQUALS;
		case 219: return KEY_L_BRACKET;
		case 221: return KEY_R_BRACKET;
		case 186: return KEY_COLON;
		case 222: return KEY_QUOTE;
		case 188: return KEY_LT;
		case 190: return KEY_GT;
		case 191: return KEY_SLASH;

        default: return KEY_UNKNOWN;
	}
}
