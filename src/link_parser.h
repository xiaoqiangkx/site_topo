#ifndef _LINKPARSER_H
#define _LINKPARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <regex.h>
#include "urllink.h"

/*
struct url_list {
	char htm[256];
	struct url_list *next;
};


typedef struct url_list urllist_t;
typedef urllist_t * urllist_p;

int get_href(char *buf);
*/
void get_info_from_url(char *host, char *path, const char *srcUrl);
//void extract_link_from_regex(char *htmltxt, link_t *q);
void extract_link_from_autom(const char * const htmltxt, link_t *linklist, const char *abpath);
//void free_urllist(queue_t *head);
//int isNotInList(const char * htmltxt, queue_t *urllist);

#endif

