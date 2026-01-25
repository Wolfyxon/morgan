#include "x11input.h"

// TODO: Display errors as strings

X11Context x11_init() {
    Display* disp = XOpenDisplay(NULL);

    if(!disp) {
        fprintf(stderr, "error: Failed to open Xorg display\n");
        exit(1);
    }

    int opcode;
    int event;
    int error;

    if(!XQueryExtension(disp, "XInputExtension", &opcode, &event, &error)) {
        fprintf(stderr, "error: Failed to query X input extension. Error: %d\n", error);
        exit(1);
    }

    XIEventMask mask;
    unsigned char mask_data[XIMaskLen(XI_LASTEVENT)] = {0};

    XISetMask(mask_data, XI_KeyPress);
    XISetMask(mask_data, XI_KeyRelease);

    mask.deviceid = XIAllDevices;
    mask.mask_len = sizeof(mask_data);
    mask.mask = mask_data;

    XISelectEvents(disp, DefaultRootWindow(disp), &mask, 1);
    XFlush(disp);

    X11Context ctx = {
        .display = disp,
        .opcode = opcode
    };

    return ctx;
}

void x11_poll_events(X11Context* ctx) {
    XEvent event;
    XNextEvent(ctx->display, &event);

    XGenericEventCookie* cookie = &event.xcookie;

    if(cookie->type == GenericEvent && cookie->extension == ctx->opcode && XGetEventData(ctx->display, cookie)) {
        if(cookie->evtype == XI_KeyPress) {
            XIDeviceEvent* dev_event = (XIDeviceEvent*)cookie->data;

            printf("Key %u from %d\n", dev_event->detail, dev_event->sourceid);
        }

        XFreeEventData(ctx->display, cookie);
    }
}

Key x11_input_to_key(int code) {
    // help me
    switch(code) {
        case 10: return KEY_1;
        case 11: return KEY_2;
        case 12: return KEY_3;
        case 13: return KEY_4;
        case 14: return KEY_5;
        case 15: return KEY_6;
        case 16: return KEY_7;
        case 17: return KEY_8;
        case 18: return KEY_9;
        case 19: return KEY_0;
        case 20: return KEY_MINUS;
        case 21: return KEY_EQUALS;

        case 24: return KEY_Q;
        case 25: return KEY_W;
        case 26: return KEY_E;
        case 27: return KEY_R;
        case 28: return KEY_T;
        case 29: return KEY_Y;
        case 30: return KEY_U;
        case 31: return KEY_I;
        case 32: return KEY_O;
        case 33: return KEY_P;
        case 34: return KEY_L_BRACKET;
        case 35: return KEY_R_BRACKET;

        case 38: return KEY_A;
        case 39: return KEY_S;
        case 40: return KEY_D;
        case 41: return KEY_F;
        case 42: return KEY_G;
        case 43: return KEY_H;
        case 44: return KEY_J;
        case 45: return KEY_K;
        case 46: return KEY_L;
        case 47: return KEY_COLON;
        case 48: return KEY_QUOTE;

        case 52: return KEY_Z;
        case 53: return KEY_X;
        case 54: return KEY_C;
        case 55: return KEY_V;
        case 56: return KEY_B;
        case 57: return KEY_N;
        case 58: return KEY_M;
        case 59: return KEY_LT;
        case 60: return KEY_GT;
    }
}
