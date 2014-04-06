/*****
** http_client.c
** - implements the methods declared in http_client.h
** - 封装处理http协议
*/

#include "http_client.h"

void mysplit(char **dest, char *src, const char *del)
{
	char *s = strtok(src, del);
	while(s!=NULL) {
		*dest++ = s;
		s = strtok(NULL, del);
	}
}

static int get_status(const char *src)
{
	int i = 0;
	char *p;
	char temp[5];
	p = strstr(src," ");
	if(p == NULL) {
		return -1;
	} else {
		p++;
		while(*p != ' '){
			temp[i++] = *p;
			p++;
		}
		temp[i]='\0';
		return atoi(temp);
	}
}

int http_init(http_client_t *http_client, const char *hostname)
{
	if(open_tcp(&http_client->network, hostname, 80) == -1)
		return -1;
	http_client->status = -1;
	http_client->content_length = -1;
	bzero(http_client->content_text, 400001);
	//fprintf(stderr,"http init success!\n");
	return 0;
}

/*返回0发送成功
 * -1失败
 */
int http_do_get(http_client_t http_client, const char *host, const char *path)/* 用get命令请求远程服务器的网页*/
{
	char tempbuf[1024];
	char req[1024];
	int flag;
	sprintf(tempbuf,"GET %s HTTP/1.0\r\n",path);
	memcpy(req,tempbuf,strlen(tempbuf));
	req[strlen(tempbuf)] = '\0';
	
	sprintf(tempbuf,"Host: %s\r\n",host);
	strcat(req,tempbuf);
	strcat(req,"Content-Type: application/x-www-form-urlencoded\r\n");
	strcat(req,"User-Agent:Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.4 (KHTML, like Gecko) Chrome/22.0.1229.94 Safari/537.4\r\n");
	
	strcat(req,"\r\n");
	
	/*
	printf("ooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
	printf("%s",req);
	printf("ooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
	*/
	flag = nsend(req, http_client.network);
	if(flag < 0)
		return -1;
	else 
		return flag;
	//fprintf(stderr, "http do_get success!\n");
}

int http_response_status(http_client_ptr_t http_client)/* 远程WEB服务器的http响应代码，如404*/
{
	//char *temp[5];
	int flag;
	char recvbuf[RECVSIZE+1];
	bzero(recvbuf,RECVSIZE+1);
	flag = recv_line(recvbuf, &(http_client->network));
	if(flag == -1) {
		fprintf(stderr, "Error in http_response_status(). recv error.\n");
		return flag;
	}
	else if (flag < 2) {
		fprintf(stderr, "Error in http_response_status(). recv error.\n");
		fprintf(stderr, "Now flag is %d, recvbuf is %s.\n",flag ,recvbuf);
		return -1;
	}
	//mysplit(temp, recvbuf, " ");
	http_client->status = get_status(recvbuf);
	bzero(recvbuf,RECVSIZE);
	flag = recv_line(recvbuf, &(http_client->network));
	if(flag <= -1){
		fprintf(stderr, "Error in http_response_status(). recv error.\n");
		return -1;
	}
	while(strcmp(recvbuf,"\r\n")!=0) {
//		mysplit(temp, recvbuf, ":");
//		if(strcasecmp(temp[0], "Content-Length")==0) {
//			http_client->content_length = atol(temp[1]);
//		}
//		bzero(recvbuf,RECVSIZE);
		flag = recv_line(recvbuf, &(http_client->network));
		if(flag <= -1){
			fprintf(stderr, "Error in http_response_status(). recv error.\n");
			return -1;
		}
	}
	//fprintf(stderr,"http get status success!\n");
	return http_client->status;
}

int http_response_body(http_client_ptr_t http_client)/*获取http响应的消息体字节流*/
{
	char tempbuf[RECVSIZE];
	int recvlen;
	int totallen = 0;
	/*
	if(http_client->content_length == -1) {
		if((http_client->content_text = (char *)malloc(sizeof(char)*400000)) == NULL) {
			fprintf(stderr,"Error in http_response_body(). content_text malloc failed.\n");
			return -1;
		}
	} else {
		if((http_client->content_text = (char *)malloc(sizeof(char)*(http_client->content_length) + 1))==NULL) {
			fprintf(stderr,"Error in http_response_body(). content_text malloc failed.\n");
			return -1;
		}
	}
	*/
	http_client->content_text[0] = '\0';
//	printf("malloc in 0x%08x. http_client->content_text malloc in http_client.\n", http_client->content_text);

	while((recvlen = nrecv(tempbuf, RECVSIZE, &http_client->network)) > 0) {
		totallen += recvlen;
		if(totallen >= 400000) {
			strncat(http_client->content_text, tempbuf,400000 - (totallen - recvlen));
			totallen = 400000;
			break;
		} else {
		strncat(http_client->content_text, tempbuf,recvlen);
//		memcpy(http_client->content_text+totallen, tempbuf, recvlen);
		totallen += recvlen;
		}
	}
	http_client->content_text[totallen] = '\0';
	if(recvlen <= -1)
		return -1;
	//fprintf(stderr,"http get body success!\n");
	return totallen;
}

void http_close(http_client_ptr_t http_client)
{
	http_client->status = -1;
	http_client->content_length = -1;
	/*
	if(http_client->content_text != NULL){
		free(http_client->content_text);
//		printf("free in 0x%08x. http_client->content_text free in http_client.\n", http_client->content_text);

		http_client->content_text = NULL;
	}
	*/
	closeSocket(&(http_client->network));
	//fprintf(stderr,"http close success!\n");
}
