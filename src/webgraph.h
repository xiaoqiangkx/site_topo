#ifndef WEBGRAPH_H
#define WEBGRAPH_H

#define URL_MAX 256
#define VERTEX_MAX 1024
#define HASH_MAX 200000

/*
 * the key node to store url_to_num node
 * only one copy of url stored
 */
typedef struct _url_to_num_t {

    int num;
    char url[URL_MAX];
    struct _url_to_num_t* next;
} url_to_num_t;

/*
 * edge node
 */
typedef struct _sm_edge_node {

    url_to_num_t* url_num_node;
    struct _sm_edge_node* next;
} sm_edge_node;


/*
 * sm vertex
 */
typedef struct _sm_vertex_t {

    url_to_num_t* url_num_node;
    struct _sm_edge_node* next;
} sm_vertex_t;

typedef struct _url_dict_t {

    url_to_num_t* next;
} url_dict_t;

/*
 * the core webgraph structure
 */
typedef struct _webg_t {
    sm_vertex_t* sm_vertex_dict;
    url_dict_t* url_dict;
    int vertex_num;
} webg_t;


/*
 * allocate memory and initialization
 * Return: success then return 0, otherwise -1
 */

int init_webg(webg_t**);


/*
 * destory sm_vertex_dict
 */
void destory(webg_t*);

#endif
