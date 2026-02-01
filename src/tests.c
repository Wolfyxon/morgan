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

char* example_config = "[test_section]\nhello = hi\nhi = 2\n[other_section]\nsomething = 123";

void test_config_slice() {
    asset_str_eq(
        config_get_section_slice(example_config, "test_section"), 
        "\nhello = hi\nhi = 2\n"
    );

    asset_str_eq(
        config_get_section_slice(example_config, "other_section"), 
        "\nsomething = 123"
    );
}
