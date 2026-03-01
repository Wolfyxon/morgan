#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "utils.h"
#include "config.h"
#include "fs.h"

// Passed to gcc via -D<name>
//#define ENABLE_TESTS
//#define TESTS_ONLY

void run_tests();

#ifdef ENABLE_TESTS

#define run_test(function) do { \
    printf("-- Running '%s' -- \n", #function); \
    function(); \
} while(0);

void test_test();
void test_trim();
void test_fexists();
void test_create_dir();
void test_rmdir();
void test_config_slice();
void test_config_entry();
void test_config_get_value();
void test_config_get_sections();

#endif
