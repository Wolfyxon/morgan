#include "tests.h"

void run_tests() {
    #ifdef ENABLE_TESTS

    printf("===== RUNNING TESTS =====\n");

    run_test(test_test);
    run_test(test_trim);
    run_test(test_crlf);
    run_test(test_fexists);
    run_test(test_create_dir);
    run_test(test_rmdir);
    run_test(test_config_slice);
    run_test(test_config_entry);
    run_test(test_config_get_value);
    run_test(test_config_get_sections);
    run_test(test_config_get_keyboards);
    
    printf("==== ALL TESTS PASSED ====\n");

    #ifdef TESTS_ONLY
    exit(0);
    #endif

    #endif
}

#ifdef ENABLE_TESTS

void test_test() {
    // Testing works lol
}

void test_trim() {
    assert_str_eq(strtrim(" "), "");
    assert_str_eq(strtrim(""), "");
    assert_str_eq(strtrim("hello "), "hello");
    assert_str_eq(strtrim(" hi\n"), "hi");
    assert_str_eq(strtrim(" abcdefg               \n"), "abcdefg");
}

void test_crlf() {
    char* crlf = "hello\r\nworld";
    char* lf = strtolf(crlf);

    assert_str_eq(lf, "hello\nworld");
    free(lf);
}

void test_fexists() {
    assert(fexists("Makefile"), "'Makefile' detected as nonexistent");
    assert(fexists("src"), "'src' dir detected as nonexistent");
}

void test_create_dir() {
    if(fexists("temp")) {
        fprintf(stderr, "warning: 'temp' already exists. Running test_rmdir() to remove it\n");
        test_rmdir();
    }

    assert(create_dir("temp") == 0, "Failed to create dir 'temp'");
    assert(fexists("temp"), "create_dir() returned success but 'temp' does not exist");
}

void test_rmdir() {
    assert(rmdir("temp") == 0, "Failed to remove dir 'temp'");
    assert(!fexists("temp"), "rmdir() returned success but 'temp' still exists");
}

#define EXAMPLE_CONFIG "[test_section]\nhello = hi\nhi = 2\n[other_section]\nsomething = 123"
#define EXAMPLE_KEYBOARDS "[keyboard_001]\r\noctave = 1 \r\n[keyboard_2]\r\noctave = 1\r\n"

void test_config_slice() {
    assert_str_eq(
        config_get_section_slice(EXAMPLE_CONFIG, "test_section"), 
        "\nhello = hi\nhi = 2\n"
    );

    assert_str_eq(
        config_get_section_slice(EXAMPLE_CONFIG, "other_section"), 
        "\nsomething = 123"
    );
}

void test_config_entry() {
    ConfigEntry* entries[] = {
        config_get_entry("abc = defg"),
        config_get_entry("abc=defg"),
        config_get_entry("\nabc =defg"),
        config_get_entry("abc= defg"),
        config_get_entry("abc = defg \n"),
        config_get_entry("     abc= defg")
    };

    size_t len = sizeof(entries) / sizeof(ConfigEntry*);
    bool fail = false;

    for(size_t i = 0; i < len; i++) {
        printf("Testing entry %lu \n", i);
        ConfigEntry* entry = entries[i];

        if(entry == NULL) {
            fprintf(stderr, "Entry %lu: Got NULL \n", i);
            fail = true;
        }

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
        config_free_entry(entries[i]);
    }

    if(fail) {
        exit(1);
    }
}

void test_config_get_value() {
    assert_str_eq(config_get_value(EXAMPLE_CONFIG, "test_section", "hello"), "hi");
    assert_str_eq(config_get_value(EXAMPLE_CONFIG, "test_section", "hi"), "2");
    assert_str_eq(config_get_value(EXAMPLE_CONFIG, "other_section", "something"), "123");
}

void test_config_get_sections() {
    size_t len = 0;
    char** sections = config_get_sections(EXAMPLE_CONFIG, &len);

    if(len != 2) {
        fprintf(stderr, "Expected len 2 got %ld\n", len);
        exit(1);
    }

    assert_str_eq(sections[0], "test_section");
    assert_str_eq(sections[1], "other_section");
    
    config_free_section_list(sections, len);
}

void test_config_get_keyboards() {
    size_t len = 0;
    KeyboardConfig* kbs = config_get_keyboards(EXAMPLE_KEYBOARDS, &len);

    if(len != 2) {
        fprintf(stderr, "Expected len 2, got %ld\n", len);
        exit(1);
    }

    assert(kbs[0].id == 1, "Unexpected ID of keyboard 1");
    assert(kbs[1].id == 2, "Unexpected ID of keyboard 2");
}

#endif