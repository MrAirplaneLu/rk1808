/*********************************************************************************
 *      Copyright:  (C) 2018 Yujie
 *                  All rights reserved.
 *
 *       Filename:  server_SSL.c
 *    Description:  server_SSL.c
 *                 
 *        Version:  1.0.0(04/21/2018)
 *         Author:  yanhuan <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "04/21/2018 09:21:24 PM"
 *                 
 ********************************************************************************/
/*********************************************************************************
 *  server端显示格式:
 *  发送时:
 *        port  :
 *        send  ：
 *
 *  接收时:
 *        port  :
 *        recv  ：
 ********************************************************************************/

#include "server_SSL.h"

SSL_CTX            *ssl_ctx;


int creat_server_SSL(info_p info_set)
{
	info_set.connect_fd = -1;    //用于服务器端连接
	info_set.accept_fd  = -1;    //用于服务器端传递信息
	pthread_t           tid;

#if	  1
	printf("\nserver_SSL主线程中\n");
	printf("打印信息如下\n");
	printf("域名或者IP:%s \n",info_set.servaddr);
	printf("端口号    :%d \n",info_set.port);
#endif	

	//SSL初始化
	SSL_library_init();                                       //SSL库引入
	OpenSSL_add_all_algorithms();                             //openssl算法导入
	SSL_load_error_strings();                                 //错误信息描述加载
	ssl_ctx = SSL_CTX_new(SSLv23_server_method());            //支持SSLv2 v3版本 server
	if(NULL == ssl_ctx)
	{
		printf("OpenSSL load failure: %s\n", strerror(errno));  //判断SSL出错信息
		exit(0);
	}
	printf("server端SSL初始化完成\n");

	/*  载入用户的数字证书， 此证书用来发送给客户端。 证书里包含有公钥 */
	if (SSL_CTX_use_certificate_file(ssl_ctx, "./cacert.pem", SSL_FILETYPE_PEM) <= 0)
	{
		printf("Load use_cert failure: %s\n", strerror(errno));
		exit(0);
	}
	/*  载入用户私钥 */
	if (SSL_CTX_use_PrivateKey_file(ssl_ctx, "./privkey.pem", SSL_FILETYPE_PEM) <= 0)
	{
		printf("Load use_Priv failure: %s\n", strerror(errno));
		exit(0);
	}
	/*  检查用户私钥是否正确 */
	if (!SSL_CTX_check_private_key(ssl_ctx))
	{
		printf("Check use_Priv error: %s\n", strerror(errno));
		exit(0);
	}  
	printf("server端证书和密钥加载完成\n");

	//创建套接字
	info_set.connect_fd = socket(AF_INET,SOCK_STREAM,0);            //创建套接字，ipV4地址，可靠的面向连接的字符流，最后0是默认协议
	if(info_set.connect_fd < 0)
	{
		printf("Create socket failure: %s\n", strerror(errno));       //如果创建失败，打印错误信息
		return -1;
	}
	//printf("Socket create fd[%d]\n",info_set.connect_fd);         //如果成功，返回fd

	//服务器设置，设置server地址和端口号，用于接受任何client_SSL端
	memset(&info_set.sock_addr,0,sizeof(info_set.sock_addr));       //清空再设置ipV4地址
	info_set.sock_addr.sin_family = AF_INET;                        //设置为ipV4地址
	info_set.sock_addr.sin_port = htons(info_set.port);             //端口号，主机字节序转网络字节序,short是16位
	info_set.sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);         //设置host主机地址，INADDR_ANY是指定为0.0.0.0的地址，表示不确定地址，或指所有地址

	//端口复用，解决进程退出后一段时间地址被占用的问题
	int           opt = 1;
	setsockopt(info_set.connect_fd,SOL_SOCKET,SO_REUSEADDR,(const void *)&opt,sizeof(opt) );

	if( bind(info_set.connect_fd, (struct sockaddr *)&info_set.sock_addr, sizeof(info_set.sock_addr)) < 0 )
	{
		printf("Bind socket failure: %s\n", strerror(errno));         //如果绑定失败，打印错误信息
		return -1;
	}

	//优化最大文件描述符
	struct  rlimit  rt;
	rt.rlim_cur = 65000 ;			//软件上限
	rt.rlim_max = 65000 ;     //硬件上限
	if(setrlimit(RLIMIT_NOFILE,&rt) == 0)
	{
		printf("Set up the maximum file descriptor：%d\n",(int)rt.rlim_cur);
	}	
	else
	{
		printf("Set up the maximum file descriptor failure: %s\n", strerror(errno));
		return -1;
	}	


	//设置排队等待连接数
	if(listen(info_set.connect_fd,255) != 0)
	{
		printf("Listen socket failure：%s\n",strerror(errno));
	}	

	while(1)
	{
		//printf("Start accept %d...\n",info_set.connect_fd);

		info_set.accept_fd = accept(info_set.connect_fd,NULL,NULL);
		if(info_set.accept_fd < 0)
		{
			printf("Accept Socket_SSL failure: %s\n", strerror(errno)); 
			return -1;  
		}
		//printf("Socket_SSL Accept fd[%d]\n",info_set.accept_fd);    //如果成功，返回fd  	

		//创建线程		
		pthread_create(&tid,NULL,server_pthreads,&info_set);

		//printf("主线程等待连接,不能关闭线程的accept_fd\n");

	}

	SSL_CTX_free(ssl_ctx);

	return 0;
}

void *server_pthreads(void *argv)
{	
	usleep(10000);
	//printf("\nserver_SSL子线程负责收发信息\n");
	int                 stReval;
	struct timeval      st_time;
	fd_set              stReadFds,stExcpFds;
	info_p              info_A;
	info_p             *info_set = NULL;

	memset(&info_A,0,sizeof(info_p));
	memcpy(&info_A,(info_p *)argv,sizeof(info_p));
	info_set = &info_A;

#if	  1
	printf("server_SSL子线程中:\n");
	//printf("打印信息如下\n");
	printf("域名或者IP:%s \n",info_set->servaddr);
	printf("端口号    :%d \n",info_set->port);
#endif		

	//基于ctx产生一个新的ssl，将连接的fd加入到ssl中，建立ssl连接
	SSL                *ssl;                                          //获取fd的方法:int SSL_get_fd(const SSL *ssl);
	ssl = SSL_new(ssl_ctx);
	SSL_set_fd(ssl,info_set->accept_fd);
	if( SSL_accept(ssl) == -1 )
	{
		printf("Accept SSL_socket failure: %s\n", strerror(errno)); 
		close(info_set->accept_fd);
	}      
	else
		//printf("Accept SSL_socket success\n");

		//处理数据收发
		while(1)
		{ 
			int retval = 0;
			//清空输入、输出
			memset(info_set->str_recv,0,sizeof(info_set->str_recv));
			memset(info_set->str_send,0,sizeof(info_set->str_send));		

			FD_ZERO(&stReadFds);
			FD_ZERO(&stExcpFds);

			FD_SET(SSL_get_fd(ssl),&stReadFds);
			FD_SET(SSL_get_fd(ssl),&stExcpFds);

			st_time.tv_sec   = (time_t) ST_TIME_S ;
			st_time.tv_usec  = (time_t) ST_TIME_US;


			stReval = select(SSL_get_fd(ssl) + 1,&stReadFds,NULL,&stExcpFds,&st_time);
			if(stReval > 0)
			{
				if( 0 != FD_ISSET(SSL_get_fd(ssl),&stReadFds) )                                     //从SSL_client读
				{

					//printf("Reading from SSL_client\n");
					if( (retval = SSL_read(ssl,info_set->str_recv,sizeof(info_set->str_recv))) == 0 )//read()出错或者断开时，返回0或-1
					{
						printf("socket_server断开连接: %s\n",strerror(errno));
						break;
					}		
					else if(retval > 0)
					{
						printf("port :%d  \n",(int)info_set->port);
						printf("recv :%s\n\n",info_set->str_recv);

						char s[4]; 
						memset(s,0,sizeof(s));                                                          //判断"quit"
						strncpy(s,info_set->str_recv,sizeof(s));
						if( 0 == strcmp("quit",s) )
						{ 
							printf("\n对方请求退出\n");
							break;
						}

#if REVERSE 					
						//逆序
						memcpy(info_set->str_send,reverse(info_set->str_recv),sizeof(info_set->str_recv));
						printf("Ready to revert\n");
						printf("send :%s\n\n",info_set->str_send);
						//将逆序序列返回
						SSL_write(ssl,info_set->str_send,sizeof(info_set->str_send));
#endif						
					}	
					else
					{
						printf("server_SSL读取出错: %s\n",strerror(errno));
						break;
					}				
				}		
			}
		}

	usleep(10000);  
	close(info_set->accept_fd);
	SSL_shutdown(ssl);
	SSL_free(ssl);

	return 0;
}

void ShowCerts(SSL * ssl)
{
	X509              *cert;             //基于X509创建的证书
	char              *line;

	cert = SSL_get_peer_certificate(ssl);
	if (cert != NULL) 
	{
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


int show_info(info_p info_set)
{
	printf("打印信息如下\n");
	printf("域名或者IP:%s \n",info_set.servaddr);
	printf("端口号    :%d \n",info_set.port);

	return 0;
}


info_p *set_info(void * info_p_set,int argc, char **argv)
{
	info_p         * info_set = (info_p *) info_p_set;

	if(1 == argc)    
	{
		info_set->port = (unsigned short)PORT_SSL;
	}
	else if(2 == argc)
	{
		if(!isnum(argv[1]))
		{
			printf("请检查端口号\n");
			exit(0);
		}
		info_set->port = (unsigned short)atoi(argv[1]);	
	}
	else 
	{	
		printf("服务端正确格式如下:");
		printf("%s 127.0.0.1 12000 \n",argv[0]);
		exit(0);
	}

	//清空发送、接收空间
	memset(info_set->str_send,0,sizeof(info_set->str_send));
	memset(info_set->str_recv,0,sizeof(info_set->str_recv));

	return info_set;
}

int isnum(char *str)
{
	int i = 0;

	for(;i < strlen(str);i++)
	{
		if(str[i] < '0' || str[i] > '9') 
		{
			return 0;	
		}
	}
	return 1;
}


//字符串逆序函数
//输入Hello World,返回dlroW olleH
char *reverse(char *buf)
{
	int     i = 0,j = 0,z = 0;
	char    temp;

	for(i = 0;i <= strlen(buf);i++)
	{
		for(j = i;j < strlen(buf);j++)
		{
			if(buf[j+1] == ' ' || (j+2) == strlen(buf))
			{
				z = 0;
				while((i+z)<(j-z))
				{			
					temp = buf[i+z];
					buf[i+z] = buf[j-z];
					buf[j-z] = temp;
					z++;
				}
				i = j+2;
			}
		}
	}

	return buf;
}

