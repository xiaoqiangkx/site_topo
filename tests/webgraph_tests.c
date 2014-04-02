#include "minunit.h"
#include <webgraph.h>

char *test_init_webg()
{

    return NULL;
}

char* test_destroy_webg() {
    return NULL;
}

char* test_insert_vertex() {
    return NULL;
}

char* test_insert_edge() {
    return NULL;
}

char* test_insert_vertex_edge() {
    return NULL;
}

char* test_set_num() {
    return NULL;
}

char* test_get_num() {
    return NULL;
}

char* test_set_get_num() {
    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_init_webg);
    mu_run_test(test_destroy_webg);
    mu_run_test(test_insert_vertex);
    mu_run_test(test_insert_edge);
    mu_run_test(test_insert_vertex_edge);
    mu_run_test(test_set_num);
    mu_run_test(test_get_num);
    mu_run_test(test_set_get_num);

    return NULL;
}

RUN_TESTS(all_tests);

