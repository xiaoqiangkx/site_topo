#include <webgraph.h>
#include <string.h>
#include <stdlib.h>
#include <hash.h>
#include "dbg.h"

int init_webg(webg_t** webg) {

    *webg = (webg_t*)malloc(sizeof(webg_t)); 
    check_mem(*webg);

    (*webg)->sm_vertex_dict = (sm_vertex_t*)malloc(sizeof(sm_vertex_t) * VERTEX_MAX);
    check_mem((*webg)->sm_vertex_dict);

    (*webg)->url_dict = (url_dict_t*)malloc(sizeof(url_dict_t) * HASH_MAX);
    check_mem((*webg)->url_dict);

    (*webg)->vertex_num = 0;

    int i = 0;
    for (i=0; i<VERTEX_MAX; i++) {
        (*webg)->sm_vertex_dict[i].next = NULL;
        (*webg)->sm_vertex_dict[i].url_num_node = NULL;
    }

    for (i=0; i<HASH_MAX; i++) {
       (*webg)->url_dict[i].next = NULL;
    }

    return 0;

error:
    if (*webg != NULL) {
        free((*webg)->sm_vertex_dict);
        free((*webg)->url_dict);
    } 
    return -1;
}


void destroy_webg(webg_t* webg) {

    if (webg == NULL) return;
    int i = 0;

    /* destory sm_vertex_dict */
    sm_edge_node *psm, *qsm;
    if (webg->sm_vertex_dict) {
        for (i=0; i<VERTEX_MAX; i++) {
            psm = webg->sm_vertex_dict[i].next;
            while (psm != NULL) {
               qsm = psm;
               psm = psm->next;
               free(qsm); 
            }      
        } 
    }

    free(webg->sm_vertex_dict);

    /* destory url_dict */
    url_to_num_t *pu, *qu;
    if (webg->url_dict) {
       for (i=0; i<HASH_MAX; i++) {
           pu = webg->url_dict[i].next;
           
           while (pu != NULL) {
               qu = pu;
               pu = pu->next;
               free(qu);
           }   
       }
    }

    free(webg->url_dict);
    free(webg);
}


int insert_vertex(webg_t* webg, const char* url) {
    /* in case of overflow */
    if (size(webg) == URL_MAX) log_err("OVERFLOW: failed to insert vertex %s", url);


    /* insert the node into url_dict */       
    unsigned hashcode = sax_hash(url) % HASH_MAX; 
    
    url_dict_t *hash_node = &webg->url_dict[hashcode];
    url_to_num_t *new_url_num_node = malloc(sizeof(url_to_num_t));
    check_mem(new_url_num_node);
    
    /* assignment of url_to_num node */
    new_url_num_node->num = webg->vertex_num;
    memcpy(new_url_num_node->url, url, strlen(url) + 1);   /* may be flaw */
    new_url_num_node->status = webg->vertex_num;    /* success to insert data */

    new_url_num_node->next = NULL;
    
    if (hash_node->next == NULL) {

        hash_node->next = new_url_num_node;
    } else {    /* push_front */

        new_url_num_node->next = hash_node->next;
        hash_node->next = new_url_num_node;
    } 


    /* insert into sm_vertex_dict */
    webg->sm_vertex_dict[webg->vertex_num].url_num_node = new_url_num_node;
    webg->vertex_num++;

    return 0;

error:
    return -1;
}

