#include <thread_pool.h>
#include <pthread.h>
#include <stdlib.h>
#include <dbg.h>

extern thread_pool_t* thread_pool;

void* thread_routine(void* args) {

    task_t *task = NULL;
    /* get the task */
    while (1) {
        pthread_mutex_lock(&thread_pool->queue_lock);
        
        while (thread_pool->stat == OPEN && thread_pool->task_cnt == 0) {
           pthread_cond_wait(&thread_pool->queue_ready, &thread_pool->queue_lock); 
        }

        if (thread_pool->stat == SHUTDOWN) {
            pthread_mutex_unlock(&thread_pool->queue_lock);
            pthread_exit(NULL);
        }

        task = thread_pool->task_head;
        thread_pool->task_head = thread_pool->task_head->next;
        
        thread_pool->busy_cnt++;
        thread_pool->task_cnt--;
        pthread_mutex_unlock(&thread_pool->queue_lock);

        task->routine(task->args);
        free(task);
        
        pthread_mutex_lock(&thread_pool->queue_lock);
        thread_pool->busy_cnt--;
        pthread_mutex_unlock(&thread_pool->queue_lock);

    }

    return NULL;
}

int thread_pool_create(int max_thread) {
    
    thread_pool = malloc(sizeof(thread_pool_t));
    check_mem(thread_pool);
    
    thread_pool->max_thr_num = max_thread;
    thread_pool->stat = OPEN;
    thread_pool->task_cnt = 0;
    thread_pool->task_head = NULL;
    thread_pool->busy_cnt = 0;

    thread_pool->thread_ids = malloc(sizeof(pthread_t) * thread_pool->max_thr_num);

    int rc = pthread_mutex_init(&thread_pool->queue_lock, NULL);
    check(rc ==0, "init queue_lock failed");

    rc = pthread_cond_init(&thread_pool->queue_ready, NULL);
    check(rc ==0, "init queue_ready failed");

    int i = 0;
    for (i=0; i<max_thread; i++) {
        rc = pthread_create(&thread_pool->thread_ids[i], NULL, thread_routine, NULL);
        check(rc == 0, "create pthread failed");
    }

    return 0;

error:
    if (thread_pool) {
        free(thread_pool->task_head);
        free(thread_pool->thread_ids);
        free(thread_pool);
    }
    /* release memory */
    return -1;
}

void thread_pool_destroy() {
    /* join every thread, only this function can access thread */ 
    if (thread_pool->stat == SHUTDOWN) {
        log_err("shutdown twice");
    }

    pthread_mutex_lock(&thread_pool->queue_lock);
    thread_pool->stat = SHUTDOWN;
    pthread_mutex_unlock(&thread_pool->queue_lock);
    pthread_cond_broadcast(&thread_pool->queue_ready);

    int i = 0;
    for (i=0; i<thread_pool->max_thr_num; i++) {
        pthread_join(thread_pool->thread_ids[i], NULL);
    }
    free(thread_pool->thread_ids);

    pthread_mutex_destroy(&thread_pool->queue_lock);
    pthread_cond_destroy(&thread_pool->queue_ready);


    task_t *p = thread_pool->task_head, *q;
    while (p != NULL) {
        q = p;
        p = p->next;
        free(q);
    }

    free(thread_pool);
}

int thread_pool_add_task(void* (*routine)(void*), void* args) {
    if (thread_pool == NULL) {
        log_err("add task into NULL thread_pool");
        return -1;
    }

    task_t *new_task = malloc(sizeof(task_t));
    check_mem(new_task);
    new_task->routine = routine;
    new_task->args = args;
    new_task->next = NULL;  /* remember to initial it */

    pthread_mutex_lock(&thread_pool->queue_lock);
    if (thread_pool->task_head == NULL) {

        thread_pool->task_head = new_task;
    } else {

        new_task->next = thread_pool->task_head->next;
        thread_pool->task_head->next = new_task;
    }
    thread_pool->task_cnt++;

    pthread_mutex_unlock(&thread_pool->queue_lock);
    pthread_cond_signal(&thread_pool->queue_ready);

    return 0;

error:
    return -1;
}
