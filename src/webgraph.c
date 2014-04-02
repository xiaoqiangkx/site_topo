#include <webgraph.h>
#include <stdlib.h>
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
