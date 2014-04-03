#ifndef WEBGRAPH_H
#define WEBGRAPH_H

#define URL_MAX 256
#define VERTEX_MAX 200000
#define HASH_MAX 200000

/*
 * the key node to store url_to_num node
 * only one copy of url stored
 * status:
 *      >=0: success to crawl
 *      =-1: waiting for crawling
 *      =-2: is crawling
 *      =-3: failed to crawl
 *      =-4: invalid url
 */
typedef struct _url_to_num_t {

    int                   num;
    char                  url[URL_MAX];
    int                   status;     /* status of number  */
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
 * return size of webg_t
 */
#define size(A) ((A)->vertex_num)


/*
 * allocate memory and initialization
 * Return: success then return 0, otherwise -1
 */
int init_webg(webg_t**);


/*
 * destory sm_vertex_dict
 */
void destory_webg(webg_t*);


/*
 * insert vertex into the sizeth data and hash table
 * return -1 if failed, otherwise 0
 */
int insert_vertex(webg_t* webg, const char* url);

/*
 * insert edge node
 * return -1 if failed, otherwise 0
 */
int insert_edge(webg_t* webg, const char* from_url, const char* to_url);

/*
 * get status of node
 */

/*
 * set status for node
 */

/******************************************Utility Function******************************/

/*
 * insert edge: if from-vertex do not exit, then we insert a from-vertex and add to-edge to it, otherwise we  just add the to-edge to the from-vertex.
 */
int get_vertex_addr(webg_t*, const char*);

#endif
