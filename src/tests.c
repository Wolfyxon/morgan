#include "tests.h"

void run_tests() {
    #ifdef ENABLE_TESTS

    printf("===== RUNNING TESTS =====\n");

    run_test(test_test);
    run_test(test_config_slice);

    printf("==== ALL TESTS PASSED ====\n");

    #endif
}

void test_test() {
    // Testing works lol
}

void test_config_slice() {
    char* config = "[test_section]\nhello = 1\nhi = 2\n[other_section]\nsomething = 2";

    asset_str_eq(config_get_section_slice(config, "other_section"), "\nsomething = 2");
}