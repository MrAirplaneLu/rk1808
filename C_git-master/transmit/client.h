/*********************************************************************************
 *      Copyright:  (C) 2018 Yujie
 *                  All rights reserved.
 *
 *       Filename:  client.h
 *    Description:  client.h
 *                   
 *        Version:  1.0.0(04/26/2018)
 *         Author:  yanhuan <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "04/26/2018 07:16:03 PM"
 *                                  
 ********************************************************************************/

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>

#define PORT_SOCKET	    12000
#define BUFSIZE         4096
#define DOMAIN_MAX_LEN  128
#define DETAIL          1
#define ST_TIME_S       0
#define ST_TIME_US      10

typedef struct info
{
	char                servaddr[DOMAIN_MAX_LEN]; //��¼��������ip
	int                 connect_fd;               //���ڷ����������ӣ������ڿͻ��˴�����Ϣ
	int                 accept_fd;                //���ڷ������˴�����Ϣ
	struct sockaddr_in  sock_addr;      
	unsigned short      port;
	char                str_send[BUFSIZE];
	char                str_recv[BUFSIZE];
}info_p;

int    creat_client(info_p info_set);
info_p set_info(info_p info_set,int argc, char **argv);
int    isnum(char *str);

#endif








