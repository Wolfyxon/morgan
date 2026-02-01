#include <stdio.h>
#include "utils.h"
#include "config.h"

#define ENABLE_TESTS

void run_tests();

#ifdef ENABLE_TESTS

#define run_test(function) do { \
    printf(": Running '%s'\n", #function); \
    function(); \
    printf(": Test passed \n"); \
} while(0);

void test_test();

#endif
