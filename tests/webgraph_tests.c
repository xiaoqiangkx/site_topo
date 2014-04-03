#include <webgraph.h>
#include <hash.h>
#include <string.h>
#include "minunit.h"

webg_t *webg = NULL;
static char* s1 = "http://localhost/index1.html";
static char* s2 = "http://localhost/index2.html";
static char* s3 = "http://localhost/index3.html";
static char* s4 = "http://localhost/index4.html";
static char* s5 = "http://localhost/index5.html";
static char* s6 = "http://localhost/index6.html";

char *test_init_webg() {
    
    init_webg(&webg);
    mu_assert(webg != NULL, "webg == NULL");
    mu_assert(webg->sm_vertex_dict != NULL, "failed to allocate memory for sm_vertex_dict.");
    mu_assert(webg->url_dict != NULL, "failed to allocate memory for url_dict.");
    mu_assert(webg->vertex_num == 0, "the number of vertex for webg is not zero.");

    return NULL;
}

char* test_destroy_webg() {

    destroy_webg(webg);
    return NULL;
}

/*
 * make sure sm_vertex_dict and url_dict are inserted with data
 */
char* test_insert_vertex() {

    insert_vertex(webg, s1);
    mu_assert(size(webg) == 1, "size of webg is not 1");
    mu_assert(webg->sm_vertex_dict[0].url_num_node->num == 0, "the num of s1 is 0"); 
  //  debug("s1 is %s", s1);
  //  debug("data in sm is %s", webg->sm_vertex_dict[0].url_num_node->url);
    mu_assert(!strncmp(s1, webg->sm_vertex_dict[0].url_num_node->url, strlen(s1)), "s1 is changed");
    mu_assert(webg->sm_vertex_dict[0].next == NULL, "next of webg[0] is not NULL");
    mu_assert(!strncmp(webg->url_dict[sax_hash(s1) % HASH_MAX].next->url, s1, strlen(s1)), "success to hash s1");
    mu_assert(webg->sm_vertex_dict[0].url_num_node->status == 0, "status of vertex 1 is success >=0");

    insert_vertex(webg, s2);
    mu_assert(size(webg) == 2, "size of webg is not 2");
    mu_assert(webg->sm_vertex_dict[1].url_num_node->num == 1, "the num of s1 is 1"); 
    mu_assert(!strncmp(s2, webg->sm_vertex_dict[1].url_num_node->url, strlen(s2)), "s2 is changed");
    mu_assert(webg->sm_vertex_dict[1].next == NULL, "next of webg[0] is not NULL");
    mu_assert(!strncmp(webg->url_dict[sax_hash(s2) % HASH_MAX].next->url, s2, strlen(s2)), "success to hash s2");
    mu_assert(webg->sm_vertex_dict[1].url_num_node->status == 1, "status of vertex 2 is success >=0");

    insert_vertex(webg, s3);
    mu_assert(size(webg) == 3, "size of webg is not 3");
    mu_assert(webg->sm_vertex_dict[2].url_num_node->num == 2, "the num of s1 is 2"); 
    mu_assert(!strncmp(s3, webg->sm_vertex_dict[2].url_num_node->url, strlen(s3)), "s3 is changed");
    mu_assert(webg->sm_vertex_dict[2].next == NULL, "next of webg[0] is not NULL");
    mu_assert(!strncmp(webg->url_dict[sax_hash(s3) % HASH_MAX].next->url, s3, strlen(s3)), "success to hash s3");
    mu_assert(webg->sm_vertex_dict[2].url_num_node->status == 2, "status of vertex 3 is success >=0");

    insert_vertex(webg, s4);
    mu_assert(size(webg) == 4, "size of webg is not 4");
    mu_assert(webg->sm_vertex_dict[3].url_num_node->num == 3, "the num of s1 is 3"); 
    mu_assert(!strncmp(s4, webg->sm_vertex_dict[3].url_num_node->url, strlen(s4)), "s4 is changed");
    mu_assert(webg->sm_vertex_dict[3].next == NULL, "next of webg[0] is not NULL");
    mu_assert(!strncmp(webg->url_dict[sax_hash(s4) % HASH_MAX].next->url, s4, strlen(s4)), "success to hash s4");
    mu_assert(webg->sm_vertex_dict[3].url_num_node->status == 3, "status of vertex 4 is success >=0");

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
    mu_run_test(test_insert_vertex);
    mu_run_test(test_insert_edge);
    mu_run_test(test_insert_vertex_edge);
    mu_run_test(test_set_num);
    mu_run_test(test_get_num);
    mu_run_test(test_set_get_num);
    mu_run_test(test_destroy_webg);

    return NULL;
}

RUN_TESTS(all_tests);

