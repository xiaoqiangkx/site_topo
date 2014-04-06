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


void mysplit(char **dest, char *src, const char *del);/*�Լ�ʵ�ֵ�split����*/
int http_init(http_client_t *http_client, const char *hostname);
int http_do_get(http_client_t http_client, const char * host,const char *path);/* ��get��������Զ�̷���������ҳ*/
int http_response_status(http_client_ptr_t http_client);/* Զ��WEB��������http��Ӧ���룬��404*/
int http_response_body(http_client_ptr_t http_client);/*��ȡhttp��Ӧ����Ϣ���ֽ���*/
void http_close(http_client_ptr_t http_client);

#endif

