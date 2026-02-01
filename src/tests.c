#include "tests.h"

void run_tests() {
    #ifdef ENABLE_TESTS

    printf("===== RUNNING TESTS =====\n");

    run_test(test_test);
    
    printf("==== ALL TESTS PASSED ====\n");

    #endif
}

void test_test() {
    // Testing works lol
}
