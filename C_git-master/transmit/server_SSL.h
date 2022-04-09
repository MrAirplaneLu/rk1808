/*********************************************************************************
 *      Copyright:  (C) 2018 Yujie
 *                  All rights reserved.
 *
 *       Filename:  server_SSL.h
 *    Description:  server_SSL.h
 *                 
 *        Version:  1.0.0(04/21/2018)
 *         Author:  yanhuan <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "04/21/2018 09:21:24 PM"
 *                 
 ********************************************************************************/


#ifndef _SERVER_SSL_H_
#define _SERVER_SSL_H_

#include <sys/socket.h>  //socket头文件
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/resource.h>  //setrlimit()解除资源限制头文件

#define REVERSE         1                      //字符串逆序函数使能

#define PORT_SSL        12001
#define BUFSIZE         4096
#define DOMAIN_MAX_LEN  128
#define ST_TIME_S       0
#define ST_TIME_US      10

typedef struct info
{
	char                servaddr[DOMAIN_MAX_LEN]; //记录域名或者ip
	int                 connect_fd;               //用于服务器端连接，或用于客户端传递信息
	int                 accept_fd;                //用于服务器端传递信息
	struct sockaddr_in  sock_addr;      
	unsigned short      port;
	char                str_send[BUFSIZE];
	char                str_recv[BUFSIZE];
}info_p;


info_p  *set_info(void * info_p_set,int argc, char **argv);
int      creat_server_SSL(info_p info_set);
int      show_info(info_p info_set);
void    *server_pthreads(void * argv);
int      isnum(char *str);
void     ShowCerts(SSL * ssl);
char    *reverse(char *buf);                    //字符串逆序函数


#endif



