/*********************************************************************************
 *      Copyright:  (C) 2018 Yujie
 *                  All rights reserved.
 *
 *       Filename:  socket_client.c
 *    Description:  client.c
 *                 
 *        Version:  1.0.0(04/26/2018)
 *         Author:  yanhuan <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "04/26/2018 07:16:03 PM"
 *                 
 ********************************************************************************/
/* 
	1、创建套接字---socket
	2、发送连接请求---connect
	3、连接后，进行通信---read，write
	4、释放套接字---close
*/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define port	 12000
#define buf_size 1024
#define ip		 192.168.109.1


int main(int argc, char **argv)
{
	int					connect_fd = -1;	//客户端只需要一个套接字：用来连接服务器
	struct sockaddr_in	server_addr;		//指定服务器端地址，是一个结构体，等下要填充ipV4或者ipV6的结构
	char 				buf[buf_size];

	connect_fd = socket(AF_INET,SOCK_STREAM,0);//创建连接套接字，ipV4地址，可靠的面向连接的字符流，最后0是默认协议
	if(connect_fd < 0)
	{
		printf("Create socket failure: %s\n", strerror(errno));  //如果创建失败，打印错误
		return -1;
	}
	printf("Socket create fd[%d]\n",connect_fd);      //如果成功，返回connect_fd
	
	//连接服务器的准备,要连接的server地址和端口号
	memset(&server_addr,0,sizeof(server_addr));       //清空再设置ipV4地址
	server_addr.sin_family = AF_INET;                //设置为ipV4地址
	server_addr.sin_port = htons(port);             //端口号，主机字节序转网络字节序,short是16位
	//BSD网络软件包含inet_addr和inet_ntoa和inet_aton，用于二进制地址和‘.’分十进制字符表示，只适用于ipV4地址，即addr需要存放32位地址
	//另外提供两个新函数：inet_ntop和inet_pton同时支持ipV4和ipV6,即addr需要存放128位地址
	
//	inet_aton("ip",&server_addr.sin_addr);
	inet_aton("127.0.0.1",&server_addr.sin_addr);	//127.0.0.1被称为回环地址，默认被看成永不宕机的接口，不需要网卡就可ping同这个本地回环地址，一般用来检查本地网络协议、基本数据是否正常
	

	//连接server端
	if(connect(connect_fd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0)
	{	
		printf("connect to server failure: %s\n", strerror(errno));
		return -1;
	}

	write(connect_fd,"Hello!This message is from client.\n",strlen("Hello!This message is from client.\n"));

	memset(buf,0,sizeof(buf));
	read(connect_fd,buf,sizeof(buf));
	printf("read  '%s' from client\n",buf);
	sleep(2);
		
	close(connect_fd);	

	return 0;
}



