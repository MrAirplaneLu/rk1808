/*********************************************************************************
 *      Copyright:  (C) 2018 Yujie
 *                  All rights reserved.
 *
 *       Filename:  transmit.h
 *    Description:  Transmit between comport/socket and SSL  
 *                 
 *        Version:  1.0.0(07/30/2018)
 *         Author:  yanhuan <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "07/30/2018 10:52:59 PM"
 *                 
 ********************************************************************************/
#ifndef _TRANSMIT_H_
#define _TRANSMIT_H_

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>     //openssl函数头文件
#include <openssl/err.h>     //openssl错误信息头文件
#include <pthread.h>
#include <unistd.h>
#include <malloc.h> 
#include <sys/select.h>      //select()复用头文件
#include <sys/resource.h>    //setrlimit()解除资源限制头文件
#include <sys/time.h>
#include <netdb.h>           //getaddrinfo()域名解析函数头文件

#define BUFSIZE         4096
#define DOMAIN_MAX_LEN  128
#define PORT_SOCKET     12000
#define PORT_SSL        12001
#define DETAIL          1
#define ST_TIME_S       0
#define ST_TIME_US      10

typedef struct info
{
	char                servaddr[DOMAIN_MAX_LEN]; //记录域名或者ip
	int                 connect_fd;               //用于服务器端连接，或用于客户端传递信息
	int                 accept_fd;                //用于服务器端传递信息
	int                 other_fd;                 //传递转发数据
	struct sockaddr_in  sock_addr;      
	unsigned short      port;
}info_p;

struct  fd_set
{
	int                socket_server;
	int                client_SSL;
};

int      client_SSL(info_p *info_set);
void     ShowCerts(SSL * ssl);
int      server_socket(info_p info_set,info_p info_set_ssl_client);
void    *SSL_pthreads (void *argv);
int      show_info(info_p info_set);
info_p  *set_info_client(void * info_p_set,int argc, char **argv);
info_p  *set_info_server(void * info_p_set,int argc, char **argv);  
int      isnum(char *str);

#endif


