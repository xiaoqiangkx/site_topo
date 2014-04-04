#include "minunit.h"

char* test_() {
    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_);

    return NULL;
}

RUN_TESTS(all_tests);
