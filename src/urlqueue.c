/*****
** urlqueue.c
** - implements the methods declared in urlqueue.h
*/

#include "urlqueue.h"

void queue_init(queue_t * q)
{
	q->size = 0;
	q->front = NULL;
	q->tail = NULL;
}

int queue_size(queue_t *q)
{
    return q->size;
}

void queue_push(queue_t *q, char *url)
{
	urlq_t *temp = (urlq_t *)malloc(sizeof(urlq_t));
//	printf("malloc in 0x%08x. temp malloc in urlqueue.\n", temp);
	if(temp == NULL) {
		fprintf(stderr, "Error: allocate memory failed in func 'queue_push'.\n");
	} else {
		q->size++;
	
		bzero(temp->url,256);
		memcpy(temp->url,url,strlen(url));
		temp->url[strlen(url)] = '\0';
		temp->next = NULL;
		if(q->front == NULL){
			//temp->next = NULL;
			q->front = temp;
			q->tail = temp;
		} else {
			q->tail->next = temp;
			q->tail = q->tail->next;
		}
	}
	temp = NULL;
}

void *queue_front(queue_t *q)
{
	return q->front;
}

void queue_pop(queue_t *q, char *url)
{
	urlq_t *now = q->front;
	memcpy(url,now->url,strlen(now->url));
	url[strlen(now->url)] = '\0';
	q->front = now->next;
	q->size--;
	if(q->size == 0) {
		q->tail = q->front;
	}
	free(now);
//	printf("free in 0x%08x. now free in urlqueue.\n", now);
}

void queue_clear(queue_t *q)
{
	urlq_t *a = q->front;
	urlq_t *now;
	while(a != NULL) {
		now = a;
		a = a->next;
		free(now);
		now = NULL;
//	printf("free in 0x%08x. now free in urlqueue.\n", now);
	}
	q->front = NULL;
	q->tail = NULL;
	q->size = 0;
//	q = NULL;
}

