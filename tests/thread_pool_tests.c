#include "minunit.h"
#include <stdio.h>
#include <dbg.h>
#include <thread_pool.h>

thread_pool_t *tp = NULL;
#define MAX_THREAD 100

char* test_create() {
    int rc = thread_pool_create(&tp, MAX_THREAD);
    mu_assert(rc == 0, "Failed to create thread pool");
    mu_assert(tp != NULL, "Failed to create tp");
    mu_assert(tp->max_thr_num == MAX_THREAD, "Failed to set max_thr_num");
    mu_assert(tp->stat == SHUTDOWN, "tp is open");
    mu_assert(tp->task_head == NULL, "tp has task");
    mu_assert(tp->thread_ids != NULL, "Faile to initial thread head");

    return NULL;
}

static int a = 0;
static int b = 1;
static void* add(void* args) {
    a++;
    b++;
    printf("thread:%d\t%d\t%d\n", (int)args, a, b);
    return NULL;
}

char* test_add_tasks() {
    int i = 0;
    thread_pool_add_task(tp, add, (void*)i);
    task_t *p = tp->task_head;
    mu_assert(tp->task_cnt == 1, "tp task is not 1");

    return NULL;
}

char* test_run_thread() {
    return NULL;
}

char* test_destroy() {
    thread_pool_destroy(tp);
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
