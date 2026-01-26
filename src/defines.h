#pragma once

#define VERSION "1.0"

//#define INPUT_DEBUG

#if defined(__linux__)
    #define TARGET_LINUX
#elif defined(_WIN64) || defined(_WIN32)
    #define TARGET_WINDOWS
#else
    #error Compiling for an unsupported OS
#endif
