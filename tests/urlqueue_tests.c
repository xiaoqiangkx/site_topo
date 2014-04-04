#include "minunit.h"
#include <string.h>
#include <urlqueue.h>

queue_t *queue = NULL;
static char* s1 = "http://localhost/index1.html";
static char* s2 = "http://localhost/index2.html";
static char* s3 = "http://localhost/index3.html";
static char* s4 = "http://localhost/index4.html";
static char* s5 = "http://localhost/index5.html";
static char* s6 = "http://localhost/index6.html";
static char* s7 = "http://localhost/index7.html";
static char url[256];

char* test_init() {
    queue = malloc(sizeof(queue_t));
    mu_assert(queue->front == NULL, "the front of queue is NULL");
    mu_assert(queue->tail == NULL, "the tail of queue is NULL");
    mu_assert(queue->size == 0, "the size of queue is not 0");
    return NULL;
}

char* test_push_pop() {
    queue_push(queue, s1);
    queue_push(queue, s2);
    queue_push(queue, s3);
    mu_assert(!strncmp(queue->front->url, s1, strlen(s1)+1), "the front of queue is NULL");
    mu_assert(!strncmp(queue->tail->url, s3, strlen(s3)+1), "the tail of queue is NULL");
    mu_assert(!strncmp(queue->front->next->url, s2, strlen(s2)+1), "the mid of queue is NULL");
    mu_assert(queue->size == 3, "the size of queue is not 1");

    queue_pop(queue, url);
    mu_assert(!strncmp(url, s1, strlen(url)+1), "the s1 is not popped");
    mu_assert(queue->size == 2, "the size of queue is not 2");
    queue_pop(queue, url);
    mu_assert(!strncmp(url, s2, strlen(url)+1), "the s2 is not popped");
    mu_assert(queue->size == 1, "the size of queue is not 1");
    queue_pop(queue, url);
    mu_assert(!strncmp(url, s3, strlen(url)+1), "the s3 is not popped");
    mu_assert(queue->size == 0, "the size of queue is not 0");

    return NULL;
}

char* test_clear() {
    queue_clear(queue);
    free(queue);
    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_init);
    mu_run_test(test_push_pop);
    mu_run_test(test_clear);

    return NULL;
}

RUN_TESTS(all_tests);
