#include <webgraph.h>
#include "minunit.h"

webg_t *webg = NULL;

char *test_init_webg()
{
    init_webg(&webg);
    mu_assert(webg != NULL, "webg == NULL");
    mu_assert(webg->sm_vertex_dict != NULL, "failed to allocate memory for sm_vertex_dict.");
    mu_assert(webg->url_dict != NULL, "failed to allocate memory for url_dict.");
    mu_assert(webg->vertex_num != 0, "the number of vertex for webg is not zero.");

    return NULL;
}

char* test_destroy_webg() {
    destroy_webg(webg);
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

