#ifndef _NETWORK_H_
#define _NETWORK_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/times.h>

#define RECVSIZE 4096
struct netWork_t {
	int clientfd;
	int nowbuflen;
	char nowbuf[RECVSIZE*10];
};
typedef struct netWork_t network_t;
typedef network_t * network_ptr_t;

int open_tcp(network_t *network, const char * hostname, const int port); /*建立一个到远程服务器的socket连接*/
int nsend(const char * sendbuf, network_t ptr); /*发送字节到网络*/
int nrecv(char *recvbuf, int recvbufsize, network_ptr_t network);/*发送网络字节流*/
int recv_line(char *recvbuf, network_ptr_t network);/*从socket接收一行以回车键结束*/
void closeSocket(network_ptr_t network);/*关闭socket*/

#endif
