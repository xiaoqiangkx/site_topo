#include "minunit.h"
#include <stdio.h>
#include <dbg.h>
#include <thread_pool.h>

thread_pool_t *thread_pool = NULL;
#define MAX_THREAD 100

char* test_create() {
    int rc = thread_pool_create(MAX_THREAD);
    mu_assert(rc == 0, "Failed to create thread pool");
    mu_assert(thread_pool != NULL, "Failed to create thread_pool");
    mu_assert(thread_pool->max_thr_num == MAX_THREAD, "Failed to set max_thr_num");
    mu_assert(thread_pool->stat == OPEN, "thread_pool is shutdown");
    mu_assert(thread_pool->task_head == NULL, "thread_pool has task");
    mu_assert(thread_pool->thread_ids != NULL, "Faile to initial thread head");

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

#define THREAD_NUM 5
char* test_add_tasks() {
    int i = 0;
    
    for (i=0; i<THREAD_NUM; i++) {
        thread_pool_add_task(add, (void*)i);
    }
    
     task_t *p = thread_pool->task_head;
    
    return NULL;
}

char* test_run_thread() {
    return NULL;
}

char* test_destroy() {
    thread_pool_destroy();
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
