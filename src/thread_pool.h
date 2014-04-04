#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>

typedef struct _task_t {
    void* args;
    void* (*routine)(void*);
    struct _task_t *next;
} task_t;

typedef struct _thread_pool_t {
    int             max_thr_num;
    int             shutdown;   /* set 0 to shutdown all threads */
    int             task_cnt;    /* tasks waiting to be done */
    pthread_mutex_t queue_lock;  /* lock the thread and queue */
    pthread_cond_t  queue_ready;  /* signal other thread to continue */
    task_t*         task_head;
    pthread_t*      thread_head;       
} thread_pool_t;


/*
 * create a thread_pool
 */
int thread_pool_create(thread_pool_t** thread_pool, int max_thread);

/*
 * destroy a thread_pool
 */
void thread_pool_destroy(thread_pool_t* thread_pool);

/*
 * add a task to the task list
 */
int thread_pool_add_task(void* (*routine)(void*), void* args);
#endif
