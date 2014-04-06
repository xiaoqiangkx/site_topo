#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <dbg.h>
#include <webgraph.h>
#include <urlqueue.h>
#include <thread_pool.h>
#include <crawler.h>

webg_t *webg;
thread_pool_t *thread_pool; /* global thread_pool */
queue_t *queue;
int craw_count;

#define THREAD_MAX 100
#define URL_MAX 256
char url_copy[URL_MAX];

void* routine(void* args) {
    char *url = (void*)args;
    char temp[URL_MAX];
    int len = strlen(url);
    printf("pthread(%d):%s\n", getpid(), url);

    if (url[len-1] == '9') return NULL;
    memcpy(temp, url, len + 1);
    
    /* doSomething */
    pthread_mutex_lock(&thread_pool->queue_lock);
    temp[len-1] += 1;
    insert_edge(webg, url, temp);
    if (get_url_status(webg, temp) == -1) {
        queue_push(queue, temp);
    } else {
        log_info("already insert %s", temp);
    }

    if (temp[len-1] < '8') {
        temp[len-1] += 1;
        insert_edge(webg, url, temp);
        if (get_url_status(webg, temp) == -1) {
            queue_push(queue, temp);
        } else {
            log_info("already insert %s", temp);
        }
    }

    int rc = set_url_status(webg, url, get_vertex_addr(webg, url));
    check(rc != -4, "url:%s is not invliad", url);
    pthread_mutex_unlock(&thread_pool->queue_lock);

    return NULL;

error:
    return NULL;
}

int main(int argc, char** argv) {
    /* 1.read data */
    //char *url = "http://xiaoqiangkx.github.io/categories.html";  /* waitting for initialization */
    char *url = argv[1];

    /* 2.initial data */
    int rc = webg_init(&webg);
    check(rc == 0, "Failed to initial webg");

    rc = queue_init(&queue);
    check(rc == 0, "Failed to initial queue");

    rc = thread_pool_create(THREAD_MAX);
    check(rc == 0, "Failed to initial thread pool");

    
    /* 3. insert data into queue, url_copy should not be used by one thread */
    pthread_mutex_lock(&thread_pool->queue_lock);
    queue_push(queue, url);
    pthread_mutex_unlock(&thread_pool->queue_lock);

    /* The simpler main thread  */
    while (1) {
        pthread_mutex_lock(&thread_pool->queue_lock);
        
        if (queue_size(queue) != 0) {

            /* pop url */
            queue_pop(queue, url_copy);
            

            int num = get_url_status(webg, url_copy);
            
            /* if it is invalid then insert it */
            if (num == -4) {

                /* insert url and do the task */
                int i = insert_vertex(webg, url_copy);
                check(i != -1, "Failed to insert data");
                num = get_url_status(webg, url_copy);
            }

            /* if url is waitting for crawling, just add the task */
            if (num == -1) {
                int len = strlen(url_copy);
                char *temp = malloc(sizeof(char) * (len + 1));
                check_mem(temp);
                memcpy(temp, url_copy, len + 1);

                pthread_mutex_unlock(&thread_pool->queue_lock);
                thread_pool_add_task(do_crawler, (void*)temp); /* other will not add task between unlock and task, so it is safe */
            } else {
                pthread_mutex_unlock(&thread_pool->queue_lock);
            }

            
        } else if (queue_size(queue) == 0 && thread_pool->busy_cnt == 0) {

            pthread_mutex_unlock(&thread_pool->queue_lock);
            log_info("Finished crawling successfully.");
            break;
        }
        
        pthread_mutex_unlock(&thread_pool->queue_lock);
        usleep(100); 
    }

    thread_pool_destroy();
    queue_clear(queue);
    destroy_webg(webg);

    return 0;

error:
    return -1;
}
