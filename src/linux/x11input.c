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
