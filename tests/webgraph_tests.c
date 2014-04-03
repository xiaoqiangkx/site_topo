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
static char* s7 = "http://localhost/index7.html";

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
    mu_assert(webg->sm_vertex_dict[0].url_num_node->status == -1, "status of vertex 1 is not -1");

    insert_vertex(webg, s2);
    mu_assert(size(webg) == 2, "size of webg is not 2");
    mu_assert(webg->sm_vertex_dict[1].url_num_node->num == 1, "the num of s1 is 1"); 
    mu_assert(!strncmp(s2, webg->sm_vertex_dict[1].url_num_node->url, strlen(s2)), "s2 is changed");
    mu_assert(webg->sm_vertex_dict[1].next == NULL, "next of webg[0] is not NULL");
    mu_assert(!strncmp(webg->url_dict[sax_hash(s2) % HASH_MAX].next->url, s2, strlen(s2)), "success to hash s2");
    mu_assert(webg->sm_vertex_dict[1].url_num_node->status == -1, "status of vertex 2 is not -1");

    insert_vertex(webg, s3);
    mu_assert(size(webg) == 3, "size of webg is not 3");
    mu_assert(webg->sm_vertex_dict[2].url_num_node->num == 2, "the num of s1 is 2"); 
    mu_assert(!strncmp(s3, webg->sm_vertex_dict[2].url_num_node->url, strlen(s3)), "s3 is changed");
    mu_assert(webg->sm_vertex_dict[2].next == NULL, "next of webg[0] is not NULL");
    mu_assert(!strncmp(webg->url_dict[sax_hash(s3) % HASH_MAX].next->url, s3, strlen(s3)), "success to hash s3");
    mu_assert(webg->sm_vertex_dict[2].url_num_node->status == -1, "status of vertex 3 is not -1");

    insert_vertex(webg, s4);
    mu_assert(size(webg) == 4, "size of webg is not 4");
    mu_assert(webg->sm_vertex_dict[3].url_num_node->num == 3, "the num of s1 is 3"); 
    mu_assert(!strncmp(s4, webg->sm_vertex_dict[3].url_num_node->url, strlen(s4)), "s4 is changed");
    mu_assert(webg->sm_vertex_dict[3].next == NULL, "next of webg[0] is not NULL");
    mu_assert(!strncmp(webg->url_dict[sax_hash(s4) % HASH_MAX].next->url, s4, strlen(s4)), "success to hash s4");
    mu_assert(webg->sm_vertex_dict[3].url_num_node->status == -1, "status of vertex 4 is not -1");

    
    int r1 = get_vertex_addr(webg, s1);
    int r3 = get_vertex_addr(webg, s3);
    int r2 = get_vertex_addr(webg, s2);
    int r4 = get_vertex_addr(webg, s4);

    mu_assert(r1 < size(webg) && !strncmp(webg->sm_vertex_dict[r1].url_num_node->url, s1, strlen(s1)), 
                "failed to find s1");
    mu_assert(r2 < size(webg) && !strncmp(webg->sm_vertex_dict[r2].url_num_node->url, s2, strlen(s2)), 
                "failed to find s2");
    mu_assert(r3 < size(webg) && !strncmp(webg->sm_vertex_dict[r3].url_num_node->url, s3, strlen(s3)), 
                "failed to find s3");
    mu_assert(r4 < size(webg) && !strncmp(webg->sm_vertex_dict[r4].url_num_node->url, s4, strlen(s4)), 
                "failed to find s4");

    int r5 = get_vertex_addr(webg, s5);
    mu_assert(r5 == -1, "do not return -1 in find invalid string s5");

    /* insert twice */
    int rc = insert_vertex(webg, s1);
    mu_assert(rc == -1, "Insert string twice");

    return NULL;
}

char* test_insert_edge() {
    
    /* insert wrong edge */
    int rc = insert_edge(webg, s6, s1);
    mu_assert(rc == -1, "Do not return -1 when inserting wrong edge");


    /* insert the same url for from_url and to_url */
    rc = insert_edge(webg, s1, s1);
    mu_assert(rc == -1, "Do not return -1 when inserting same urls");


    /* insert unknown edge */
    rc = insert_edge(webg, s1, s6);
    mu_assert(rc == 0, "failed to insert unknown url");

    /* insert right url  */
    rc = insert_edge(webg, s1, s2);
    mu_assert(rc == 0, "failed to insert right url");
    rc = insert_edge(webg, s1, s3);
    mu_assert(rc == 0, "failed to insert right url");
    rc = insert_edge(webg, s1, s4);
    mu_assert(rc == 0, "failed to insert right url");
    rc = insert_edge(webg, s2, s4);
    mu_assert(rc == 0, "failed to insert right url");


    /* insert edge twice */
    rc = insert_edge(webg, s2, s4);
    mu_assert(rc == -1, "insert edge twice");
    return NULL;
}


char* test_get_status() {

    int num = get_url_status(webg, s1);
    mu_assert(IS_WAIT(num), "s1 should be waiting");

    num = get_url_status(webg, s7);
    mu_assert(IS_INVALID(num), "s7 should not be valid");
    return NULL;
}

char* test_set_status() {
    int num = set_url_status(webg, s1, -2);
    mu_assert(num == 0, "Failed to set status");
    mu_assert(IS_RUNNING(get_url_status(webg, s1)), "set status is not -2");

    num = set_url_status(webg, s1, -3);
    mu_assert(IS_FAILED(get_url_status(webg, s1)), "Failed to set status");

    num = set_url_status(webg, s7, -3);
    mu_assert(IS_INVALID(num), "Should not success to set for a unvalid url");

    return NULL;
}


char *all_tests() {
    mu_suite_start();

    mu_run_test(test_init_webg);
    mu_run_test(test_insert_vertex);
    mu_run_test(test_insert_edge);
    mu_run_test(test_get_status);
    mu_run_test(test_set_status);
    mu_run_test(test_destroy_webg);

    return NULL;
}

RUN_TESTS(all_tests);
