#include "tests.h"

void run_tests() {
    #ifdef ENABLE_TESTS

    printf("===== RUNNING TESTS =====\n");

    run_test(test_test);
    run_test(test_config_slice);
    run_test(test_config_entry);
    run_test(test_trim);

    printf("==== ALL TESTS PASSED ====\n");

    #endif
}

#ifdef ENABLE_TESTS

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

void test_config_entry() {
    ConfigEntry* entries[] = {
        config_get_entry("abc = defg"),
        config_get_entry("abc=defg"),
        config_get_entry("abc =defg"),
        config_get_entry("abc= defg"),
        config_get_entry("abc = defg \n"),
        config_get_entry("     abc= defg")
    };

    size_t len = sizeof(entries) / sizeof(ConfigEntry*);
    bool fail = false;

    for(size_t i = 0; i < len; i++) {
        ConfigEntry* entry = entries[i];

        if(strcmp(entry->key, "abc") != 0) {
            fprintf(stderr, "Entry %lu: Unexpected key '%s' \n", i, entry->key);
            fail = true;
        }

        if(strcmp(entry->value, "defg") != 0) {
            fprintf(stderr, "Entry %lu: Unexpected value '%s' \n", i, entry->value);
            fail = true;
        }
    }

    for(size_t i = 0; i < len; i++) {
        
    }

    if(!fail) {
        exit(1);
    }
}

void test_trim() {
    asset_str_eq(strtrim(" "), "");
    asset_str_eq(strtrim(""), "");
    asset_str_eq(strtrim("hello "), "hello");
    asset_str_eq(strtrim(" hi\n"), "hi");
}

#endif