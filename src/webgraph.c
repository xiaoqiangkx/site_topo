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
    if (size(webg) == URL_MAX) {
        log_err("OVERFLOW: failed to insert vertex %s", url);
        return -1;
    }
    
    /* avoid to insert twice */
    if (get_vertex_addr(webg, url) != -1) {
       log_err("Insert vertex twice, %s", url);
       return -1; 
    }


    /* insert the node into url_dict */       
    unsigned hashcode = sax_hash(url) % HASH_MAX; 
    
    url_dict_t *hash_node = &webg->url_dict[hashcode];
    url_to_num_t *new_url_num_node = malloc(sizeof(url_to_num_t));
    check_mem(new_url_num_node);
    
    /* assignment of url_to_num node */
    new_url_num_node->num = webg->vertex_num;
    memcpy(new_url_num_node->url, url, strlen(url) + 1);   /* may be flaw */
    new_url_num_node->status = -1;    /* success to insert data */

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

    return webg->vertex_num - 1;

error:
    return -1;
}

int insert_edge(webg_t* webg, const char* from_url, const char* to_url) {
    if (strncmp(from_url, to_url, strlen(to_url)) == 0) {
        log_info("from_url and to_url are both %s", from_url);    
        return -1;
    }

    /* find from_url, if not we return -1(failed) */
    int from_num = get_vertex_addr(webg, from_url);
    if (from_num < 0) {
        log_info("failed to find from_url %s", from_url);
        return -1;
    }

    /* find to_url, if not we just insert a new node to_url */    
    int to_num = get_vertex_addr(webg, to_url);
    if (to_num < 0) {
        log_info("do not find to_url %s", to_url);
        to_num = insert_vertex(webg, to_url);
        log_info("failed to insert to_url %s", to_url);
        if (to_num < 0) return -1;
    }


    /* insert the edge */
    sm_vertex_t *from_node = &webg->sm_vertex_dict[from_num];  

    /* Defensive coding: judge whether to_node aleary inserted */
    sm_edge_node *p = from_node->next;
    while (p != NULL) {
        if (strncmp(p->url_num_node->url, to_url, strlen(to_url)) == 0) return -1;
        p = p->next;
    }

    sm_edge_node *new_edge_node = malloc(sizeof(sm_edge_node));
    check_mem(new_edge_node);

    new_edge_node->next = NULL;
    new_edge_node->url_num_node = webg->sm_vertex_dict[to_num].url_num_node;

    if (from_node == NULL) {
        
        from_node->next = new_edge_node;    
    } else {

        new_edge_node->next = from_node->next;
        from_node->next = new_edge_node;
    }

    return 0;

/* if error, clean those memory */
error:
    free(new_edge_node);
    return -1;
}


int get_url_status(webg_t* webg, const char* url) {

    int num = get_vertex_addr(webg, url);
    if (num == -1) return -4;

    return webg->sm_vertex_dict[num].url_num_node->status;
}

int set_url_status(webg_t* webg, const char* url, int status) {
    
    int num = get_vertex_addr(webg, url);
    if (num == -1) return -4;

    webg->sm_vertex_dict[num].url_num_node->status = status;
    return 0;
}

/******************************************Utility Function******************************/

int get_vertex_addr(webg_t* webg, const char* url) {
    if (url == NULL) return -1;

    unsigned hashcode = sax_hash(url) % HASH_MAX; 
    
    url_dict_t* hashnode = &webg->url_dict[hashcode]; 

    if (hashnode->next == NULL) return -1;

    url_to_num_t* p = hashnode->next;

    while (p != NULL) {
        if (strncmp(url, p->url, strlen(url)) == 0) return p->num;
        
        p = p->next;
    }

    return -1;
}


