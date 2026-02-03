#pragma once

//#define INPUT_DEBUG

#ifndef VERSION
    #define VERSION "?"
#endif

#if defined(__linux__)
    #define TARGET_LINUX
#elif defined(_WIN64) || defined(_WIN32)
    #define TARGET_WINDOWS
#else
    #error Compiling for an unsupported OS
#endif
