#include "minunit.h"
#include <dbg.h>
#include <thread_pool.h>

thread_pool_t *tp = NULL;
#define MAX_THREAD 100

char* test_create() {
    int rc = thread_pool_create(&tp, MAX_THREAD);
    mu_assert(rc == 0, "Failed to create thread");

    return NULL;
}

char* test_add_tasks() {
    return NULL;
}

char* test_run_thread() {
    return NULL;
}

char* test_destroy() {
    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_add_tasks);
    mu_run_test(test_run_thread);
    mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests);
