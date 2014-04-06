/*****
** crawler.c
** - implements the methods declared in crawler.h
** -爬取线程的核心流程
*/
#include <crawler.h>
#include <dbg.h>

extern webg_t *webg;
extern queue_t *queue;
extern thread_pool_t *thread_pool;
extern int craw_count;

void *do_crawler(void *url) //完成爬取线程的核心流程
{
	char hostname[20],path[200],link[255];
	int status, num, flag = -1, count = 0, re;
	link_t  *linklist;
	http_client_t http_client;
	
	pthread_mutex_lock(&thread_pool->queue_lock);
	printf("--------------------------crawler \"%s\"-----------------------\n", (char *)url);
	printf("now has already crawlered: %d\n",craw_count++);
	printf("now queue size is %d\n",queue_size(queue));
	pthread_mutex_unlock(&thread_pool->queue_lock);
	while(flag == -1 && count < 3) {
		get_info_from_url(hostname, path,(char *)url);

		re = http_init(&http_client,hostname);
		if(re == -1) {
			flag = -1;
			count++;
			continue;
		}
		re = http_do_get(http_client,hostname,(char*)url);
		if(re == -1) {
			flag = -1;
			http_close(&http_client);
			count++;
			continue;
		}
		status = http_response_status(&http_client);
		if(status == -1) {
			flag = -1;
			http_close(&http_client);
			count++;
			continue;
		} else if(status/100 != 2) {
			flag = 1;
			break;
		} else {
			re = http_response_body(&http_client);
			if(re == -1) {
				flag = -1;
				http_close(&http_client);
				count++;
				continue;
			} else {
				flag = 1;
				break;
			}
		}
	}
	if(flag == -1) {
			pthread_mutex_lock(&thread_pool->queue_lock);
		num = set_url_status(webg, url, -3);
			pthread_mutex_unlock(&thread_pool->queue_lock);
		//printf("url is %s, set is %d.\n",(char *)url, num);
	} else if(status / 100 != 2) {
			pthread_mutex_lock(&thread_pool->queue_lock);
		num = set_url_status(webg, url, -3);
			pthread_mutex_unlock(&thread_pool->queue_lock);
		//printf("url is %s, set is %d.\n",(char *)url, num);
		http_close(&http_client);
	} else {
		linklist = (link_t *)malloc(sizeof(link_t));
//		printf("malloc in : 0x%08x. linklist malloc in crawler.\n", linklist);
		if(linklist == NULL) {
			fprintf(stderr, "Error in crawler().linklist malloc failed.\n");
			free(url);
//			printf("free in : 0x%08x. url free in crawler\n",url);
			http_close(&http_client);
			return NULL;
		}
		link_init(linklist);
		extract_link_from_autom(http_client.content_text, linklist, url);

			pthread_mutex_lock(&thread_pool->queue_lock);
		num = set_url_status(webg, url, get_vertex_addr(webg, url));
			pthread_mutex_unlock(&thread_pool->queue_lock);
		//printf("url is %s, set is %d.\n",(char *)url, num);
		while(link_size(linklist) > 0) {
			link_pop(linklist,link);
			pthread_mutex_lock(&thread_pool->queue_lock);
			num = get_url_status(webg, link);
			//printf("url is %s, num is %d.\n",link,num);
			if(num == -4) { //未找到点，插入
				insert_vertex(webg, link);
				queue_push(queue, link);
                log_info("link is %s", link);
			}
			pthread_mutex_unlock(&thread_pool->queue_lock);
			insert_edge(webg, url, link);
		}
		link_clear(linklist);
		free(linklist);
//			printf("free in : 0x%08x. linklist free in crawler\n",linklist);
		http_close(&http_client);
	}
	free(url);
//			printf("free in : 0x%08x. url free in crawler\n",url);
	return NULL;
}
