/*****
** urlqueue.c
** - implements the methods declared in urlqueue.h
*/

#include "urllink.h"

void link_init(link_t * q)
{
	q->size = 0;
	q->link = NULL;
}

int link_size(link_t *q)
{
    return q->size;
}

void link_push(link_t *q, char *url)
{
	urll_t *temp, *now, *last;
	int flag = 0;
	temp = (urll_t *)malloc(sizeof(urll_t));
//	printf("malloc in 0x%08x. temp malloc in urllink.\n",temp);
	if(temp == NULL) {
		fprintf(stderr, "Error in link_add(), temp malloc failed.\n");
		return;
	} else {
	memcpy(temp->url,url,strlen(url));
	temp->url[strlen(url)] = '\0';
	temp->next = NULL;
	
	if(q->link == NULL){
		q->link = temp;
		q->size++;
	} else {
		now = q->link;
		while(now != NULL) {
			if(strcasecmp(now->url,url) == 0) {
				flag = 1;
				break;
			}
			last = now;
			now = now->next;
		}
		if(flag == 1) {
			free(temp);
//	printf("free in 0x%08x. temp free in urllink.\n",temp);
			temp = NULL;
		} else {
			last->next = temp;
			q->size++;
		}
	}
	return;
	}
}

void link_pop(link_t *q, char *url)
{
	urll_t *now = q->link;
	memcpy(url,now->url,strlen(now->url));
	url[strlen(now->url)] = '\0';
	q->link = now->next;
	q->size--;
	free(now);
//	printf("free in 0x%08x. now free in urllink.\n",now);
}

void link_clear(link_t *q)
{
	urll_t *a = q->link;
	urll_t *now;
	while(a != NULL) {
		now = a;
		a = a->next;
		free(now);
//	printf("free in 0x%08x. now free in urllink.\n",now);
		now = NULL;
	}
	q->link = NULL;
	q->size = 0;
//	q = NULL;
}
