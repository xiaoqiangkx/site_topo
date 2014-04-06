#ifndef _URL_LINK_H_
#define _URL_LINK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _urllink {
	char url[256];
	struct _urllink *next;
} urll_t;

typedef struct _link {
    int size;
	urll_t *link;
} link_t;

/*****
** initialize an empty urlq_t
** must be called first after a new urlq_t is declared
*/
void link_init(link_t *q);


/*****
** push a new element to the end of the urlq_t
** it's up to the client code to allocate and maintain memory of "element"
*/
void link_push(link_t *q, char *url);

/*****
** remove the first element (pointer) from the urlq_t
** set "release" to non-zero if memory deallocation is desired
*/
void link_pop(link_t *q, char *url);

/*****
** remove all elements (pointers) from the urlq_t
** set "release" to non-zero if memory deallocation is desired
*/
void link_clear(link_t *q);

/*****
** return current number of elements in the urlq_t, or 0 when urlq_t is empty
*/
int link_size(link_t *q);


#endif /* _RZ_C_QUEUE_ */
