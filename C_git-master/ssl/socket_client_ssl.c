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
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <unistd.h>

#define port	 12000
#define buf_size 1024
#define ip		 192.168.109.1

void ShowCerts(SSL  *ssl);

int main(int argc, char **argv)
{
	int					connect_fd = -1;	//客户端只需要一个套接字：用来连接服务器
	struct sockaddr_in	server_addr;		//指定服务器端地址，是一个结构体，等下要填充ipV4或者ipV6的结构
	char 				buf[buf_size];
	
	SSL_CTX *ctx;

	/*  SSL 库初始化 */
	SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	ctx = SSL_CTX_new(SSLv23_client_method());
	if (ctx == NULL) 
	{
		printf("OpenSSL load failure: %s\n", strerror(errno));  //如果创建失败，打印错误信息
		return -1;
	}		
	

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

	/*  基于 ctx 产生一个新的 SSL */
	SSL			*ssl;
	ssl = SSL_new(ctx);
	SSL_set_fd(ssl,connect_fd);
	/*  建立 SSL 连接 */
	if( SSL_connect(ssl) == -1 )
	{
		printf("SSL_connect failure: %s\n",strerror(errno));
		return -1;
	}
	else
		printf("SSL_connece success: %s\n",SSL_get_cipher(ssl));
	
	ShowCerts(ssl);
	
	SSL_write(ssl,"Hello!This message is from client.\n",strlen("Hello!This message is from client.\n"));

	memset(buf,0,sizeof(buf));
	SSL_read(ssl,buf,sizeof(buf));
	printf("read  '%s' from client\n",buf);
 /*  关闭连接 */
 	SSL_shutdown(ssl);
    SSL_free(ssl);	
    SSL_CTX_free(ctx);
  
	return 0;
}

void ShowCerts(SSL * ssl)
{
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl);
    if (cert != NULL) {
        printf("数字证书信息:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("证书: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("颁发者: %s\n", line);
        free(line);
        X509_free(cert);
    } else
        printf("无证书信息！\n");
}
