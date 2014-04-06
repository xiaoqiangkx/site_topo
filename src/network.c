/*****
** network.c
** - implements the methods declared in network.h
** - 对网络socket的收发进行封装
*/

#include "network.h"


int open_tcp(network_t *network, const char * hostname, const int port)/*建立一个到远程服务器的socket连接*/
{
	struct sockaddr_in serveraddr;
	struct hostent *hp, hostbuf;
	struct timeval nTimeout = {10,0};
	int res,ret,re;
	char buffer[1024];

	if((network->clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr,"Error in open_tcp(). create socket failed.\n");
		return -1;
	}
	if((re = setsockopt(network->clientfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&nTimeout, sizeof(struct timeval)))!=0){
		fprintf(stderr,"Error in open_tcp(). Timeout set error!,error : %d\n",re);		
		return -1;
	}
    res = gethostbyname_r(hostname, &hostbuf, buffer, sizeof(buffer), &hp, &ret);
	if(res || hp == NULL){
		fprintf(stderr,"Error in open_tcp(). res or hp get failed.\n");
		return -1;
	}
	bzero((char*) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char*)hp->h_addr_list[0],(char *)&serveraddr.sin_addr.s_addr,hp->h_length);
	serveraddr.sin_port = htons(port);

	if(connect(network->clientfd, (struct sockaddr*) &serveraddr,sizeof(serveraddr)) < 0) {
		fprintf(stderr, "Error in open_tcp(). connect failed.\n");
		return -1;
	}
	bzero(network->nowbuf,sizeof(network->nowbuf));
	network->nowbuflen=0;
	return 0;
}


int nsend(const char * sendbuf, network_t network) /*发送字节到网络*/
{
	int flag = -1, count = 1;
	/*
	while(1) {
		flag = send(network.clientfd, sendbuf, strlen(sendbuf), 0);
		if(flag < 0) {
			fprintf(stderr,"send error.Time: %d\n",count);
			if(count >= 3) {
				break;
			} else {
				count++;
			}
		} else {
			break;
		}
	}
	*/
		flag = send(network.clientfd, sendbuf, strlen(sendbuf), 0);
		if(flag < 0) {
			//fprintf(stderr,"send error.Time: %d\n",count);
			flag = -1;
		}
	return flag;
}


int nrecv(char *recvbuf, int recvbufsize, network_ptr_t network)/*接收网络字节流*/
{
	int recvlen;
	int recvbuflen;
	int flag = 1, count = 1;
	char tempbuf[RECVSIZE+1];
	bzero(recvbuf,recvbufsize);
	if(recvbufsize > RECVSIZE*9)
		return -1;
	while(1) {
		if(network->nowbuflen >= recvbufsize) {
			memcpy(recvbuf,network->nowbuf,recvbufsize);
			memmove(network->nowbuf,&network->nowbuf[recvbufsize],network->nowbuflen-recvbufsize);
			network->nowbuf[network->nowbuflen-recvbufsize]='\0';
			network->nowbuflen = network->nowbuflen - recvbufsize;
			recvbuflen = recvbufsize;
			break;
		} else {
			recvlen = recv(network->clientfd, tempbuf, RECVSIZE, 0);
			if(recvlen>0) {
				memcpy(network->nowbuf + network->nowbuflen, tempbuf, recvlen);
				network->nowbuflen+=recvlen;
			} else if (recvlen <= -1) {
				fprintf(stderr,"recv timeout.");
				flag = -1;
				break;
			} else {
				memcpy(recvbuf,network->nowbuf,network->nowbuflen);
				recvbuf[network->nowbuflen] = '\0';
				recvbuflen = network->nowbuflen;
				network->nowbuflen = 0;
				break;
			} 
		}
	}
	/*
	while(1) {
		if(network->nowbuflen >= recvbufsize) {
			memcpy(recvbuf,network->nowbuf,recvbufsize);
			memmove(network->nowbuf,&network->nowbuf[recvbufsize],network->nowbuflen-recvbufsize);
			network->nowbuf[network->nowbuflen-recvbufsize]='\0';
			network->nowbuflen = network->nowbuflen - recvbufsize;
			recvbuflen = recvbufsize;
			break;
		} else {
			recvlen = recv(network->clientfd, tempbuf, RECVSIZE, 0);
			if(recvlen>0) {
				memcpy(network->nowbuf + network->nowbuflen, tempbuf, recvlen);
				network->nowbuflen+=recvlen;
			} else if (recvlen == -1) {
				fprintf(stderr,"recv timeout.Time: %d\n",count);
				if(count == 3) {
					flag = -1;
					break;
				} else {
					flag = 1;
					count++;
				}
			} else {
				memcpy(recvbuf,network->nowbuf,network->nowbuflen);
				recvbuf[network->nowbuflen] = '\0';
				recvbuflen = network->nowbuflen;
				network->nowbuflen = 0;
				break;
			} 
		}
	}
	*/
	if(flag == -1) {
		return flag;
	} else {
		return recvbuflen;
	}
}

int recv_line(char *recvbuf, network_ptr_t network)/*从socket接收一行以回车键结束*/
{

	int i,flag;
	int recvlen;
	int tag = 1, count = 1;
	int recvbuflen=0;
	char tempbuf[RECVSIZE+1];
	bzero(recvbuf,RECVSIZE+1);
	while(1) {
		flag=0;
		i=0;
		while(i<network->nowbuflen) {
			if(network->nowbuf[i]=='\n') {
				memcpy(recvbuf,network->nowbuf,i+1);
				recvbuf[i+1]='\0';
				recvbuflen=i+1;
				flag=1;
				break;
			}
			i++;
			if(i >= RECVSIZE) {
				flag = -2;
				return -1;
			}
		}
		if(flag==1) {
			memmove(network->nowbuf,&network->nowbuf[i+1],network->nowbuflen-i-1);
			network->nowbuf[network->nowbuflen-i-1]='\0';
			network->nowbuflen=network->nowbuflen-i-1;

			break;
		} else {
			recvlen=recv(network->clientfd,tempbuf,RECVSIZE,0);
			if(recvlen > 0) {
				strncat(network->nowbuf,tempbuf,recvlen);
				network->nowbuflen+=recvlen;
			} else if (recvlen == 0) {
				memcpy(recvbuf,network->nowbuf,network->nowbuflen);
				recvbuf[network->nowbuflen]='\0';
				recvbuflen=network->nowbuflen;
				network->nowbuflen=0;
				break;
			}  else {
				fprintf(stderr,"recv timeout.Time: %d\n",count);
					tag = -1; 
					break;
			}
		}
	}
	if(tag == -1)
		return -1;
	else
		return recvbuflen;
}

void closeSocket(network_ptr_t network)/*关闭socket*/
{
	network->nowbuflen=0;
	bzero(network->nowbuf,RECVSIZE*10);
	close(network->clientfd);
}
