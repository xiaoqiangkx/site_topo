#ifndef _HTTPCLIENT_H_
#define _HTTPCLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network.h"

struct httpClient_t {
	network_t network;
	int status;
	long content_length;
	char content_text[400001];
};
typedef struct httpClient_t http_client_t;
typedef struct httpClient_t *http_client_ptr_t;


void mysplit(char **dest, char *src, const char *del);/*自己实现的split函数*/
int http_init(http_client_t *http_client, const char *hostname);
int http_do_get(http_client_t http_client, const char * host,const char *path);/* 用get命令请求远程服务器的网页*/
int http_response_status(http_client_ptr_t http_client);/* 远程WEB服务器的http响应代码，如404*/
int http_response_body(http_client_ptr_t http_client);/*获取http响应的消息体字节流*/
void http_close(http_client_ptr_t http_client);

#endif

