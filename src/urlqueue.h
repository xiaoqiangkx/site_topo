#ifndef _URL_QUEUE_H
#define _URL_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _urlqueue {
	char url[256];
	struct _urlqueue *next;
} urlq_t;

typedef struct _queue {
    int size;
	urlq_t *front;
	urlq_t *tail;
} queue_t;

/*****
** initialize an empty urlq_t
** must be called first after a new urlq_t is declared
*/
int queue_init(queue_t **q);


/*****
** push a new element to the end of the urlq_t
** it's up to the client code to allocate and maintain memory of "element"
*/
void queue_push(queue_t *q, char *url);

/*****
** return the first element in the urlq_t, or NULL when the urlq_t is empty
*/
void *queue_front(queue_t *q);

/*****
** remove the first element (pointer) from the urlq_t
** set "release" to non-zero if memory deallocation is desired
*/
void queue_pop(queue_t *q, char *url);

/*****
** remove all elements (pointers) from the urlq_t
** set "release" to non-zero if memory deallocation is desired
*/
void queue_clear(queue_t *q);

/*****
** return current number of elements in the urlq_t, or 0 when urlq_t is empty
*/
int queue_size(queue_t *q);


#endif /* _RZ_C_QUEUE_ */
