#ifndef _CRAWLER_H_
#define _CRAWLER_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "http_client.h"
#include "urlqueue.h"
#include "urllink.h"
#include "webgraph.h"
#include "link_parser.h"
#include "thread_pool.h"

void *do_crawler(void *url); //完成爬取线程的核心流程

#endif

