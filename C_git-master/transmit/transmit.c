/*********************************************************************************
 *      Copyright:  (C) 2018 Yujie
 *                  All rights reserved.
 *
 *       Filename:  transmit.c
 *    Description:  Transmit between comport/socket and SSL  
 *                 
 *        Version:  1.0.0(07/30/2018)
 *         Author:  yanhuan <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "07/30/2018 10:52:59 PM"
 *                 
 ********************************************************************************/
/*********************************************************************************
 *  transmit端显示格式:
 *  从普通socket到加密server时:
 *        From socket_client to SSL_server
 *        data  :
 *
 *  从加密server到普通socket时:
 *        From SSL_server to socket_client
 *        data  :
 ********************************************************************************/

#include "transmit.h"

//创建client_SSL，并处理转发数据
int  client_SSL(info_p * info_set)
{
	info_set->connect_fd = -1;
	SSL_CTX            *client_ctx;
	char                str_data[BUFSIZE];
	int                 stReval;
	struct timeval      st_time;
	fd_set              stReadFds,stExcpFds;

#if	  DETAIL
	printf("\nclient_SSL子线程中\n");
	printf("打印信息如下\n");
	printf("域名或者IP:%s \n",info_set->servaddr);
	printf("端口号    :%d \n",info_set->port);
#endif	

	//SSL初始化
	SSL_library_init();                                //SSL库引入
	OpenSSL_add_all_algorithms();                      //openssl算法导入
	SSL_load_error_strings();                          //错误信息描述加载
	client_ctx = SSL_CTX_new(SSLv23_client_method());  //支持SSLv2 v3版本
	if(NULL == client_ctx)
	{
		printf("OpenSSL load failure: %s\n", strerror(errno));  //判断SSL出错信息
		return -1;
	}
	//printf("client端SSL初始化完成\n");

	info_set->connect_fd = socket(AF_INET,SOCK_STREAM,0);   //创建ipV4 TCP的socket
	if(info_set->connect_fd < 0)
	{
		printf("Create socket failure: %s\n", strerror(errno)); //判断socket出错信息
		return -1;
	}
	//printf("Socket create fd[%d] as client_SSL for server_SSL \n",info_set->connect_fd);


	//准备连接服务器的设置，设置地址和端口号
	memset(&info_set->sock_addr,0,sizeof(info_set->sock_addr));
	info_set->sock_addr.sin_family = AF_INET;
	info_set->sock_addr.sin_port = htons(info_set->port);
	inet_aton(info_set->servaddr,&info_set->sock_addr.sin_addr);


	//连接server_SSL
	if( 0 > connect(info_set->connect_fd,(struct sockaddr*)&info_set->sock_addr,sizeof(info_set->sock_addr)) )
	{
		printf("Connect to server failure: %s\n", strerror(errno));
		return -1;
	}

	//基于client_ctx产生一个新的SSL来接受信息
	SSL                *ssl;
	ssl = SSL_new(client_ctx);
	SSL_set_fd(ssl,info_set->connect_fd);
	if(-1 ==  SSL_connect(ssl) )
	{
		printf("SSL_connect failure: %s\n",strerror(errno));
		return -1;	
	}
	else 
		printf("SSL_connect success: %s\n",SSL_get_cipher(ssl));

	//显示证书信息
	ShowCerts(ssl);

	//处理数据收发			
	while(1)
	{
		int retval = 0;
		//清空输入、输出
		memset(str_data,0,sizeof(str_data));

		FD_ZERO(&stReadFds);
		FD_ZERO(&stExcpFds);

		FD_SET(SSL_get_fd(ssl),&stReadFds);
		FD_SET(info_set->other_fd,&stReadFds);
		FD_SET(SSL_get_fd(ssl),&stExcpFds);
		FD_SET(info_set->other_fd,&stReadFds);

		st_time.tv_sec   = (time_t) ST_TIME_S ;
		st_time.tv_usec  = (time_t) ST_TIME_US;	

		stReval = select(SSL_get_fd(ssl) + 1,&stReadFds,NULL,&stExcpFds,&st_time);         
		if(stReval > 0)
		{
			//处理从server_SSL来的数据
			if( 0 != FD_ISSET(SSL_get_fd(ssl),&stReadFds) )                  							//从server_SSL读,流向:SSL--->socket
			{
				//printf("Reading from server_SSL\n");
				if( (retval = SSL_read(ssl,str_data,sizeof(str_data))) == 0 )  //read()出错或者断开时，返回0或-1
				{
					printf("server_SSL断开连接: %s\n",strerror(errno));
					break;
				}	
				else if(retval < 0)
				{
					printf("server_SSL连接出错: %s\n",strerror(errno));
					break;
				}	
				else if(retval > 0)
				{
					printf("From SSL_server to socket_client\n");
					printf("data :%s\n\n",str_data);
					printf("开始转发 : From SSL_server to socket_client \n");
					write(info_set->other_fd,str_data,sizeof(str_data));

					char s[4];  
					memset(s,0,sizeof(s));                                                     //判断"quit"
					strncpy(s,str_data,sizeof(s));									
					if( 0 == strcmp("quit",s) )
					{
						printf("\nSSL_server方请求退出\n");
						break;
					}		
				}				
			}
			if( 0 != FD_ISSET(SSL_get_fd(ssl),&stExcpFds) )                                       //异常
			{
				printf("Error happened: %s\n",strerror(errno));
				break;
			}	

			//处理从socket_client来的数据
			if( 0 != FD_ISSET(info_set->other_fd,&stReadFds) )                                       //从socket_client读,流向:socket--->SSL
			{
				//printf("Reading from socket_client\n");
				if( (retval = read(info_set->other_fd,str_data,sizeof(str_data))) == 0 )  //read()出错或者断开时，返回0或-1
				{
					printf("socket_client断开连接: %s\n",strerror(errno));
					break;
				}	
				else if(retval < 0)
				{
					printf("socket_client连接出错: %s\n",strerror(errno));
					break;
				}	
				else if(retval > 0)
				{
					printf("From socket_client to SSL_server\n");
					printf("data :%s\n\n",str_data);
					printf("开始转发 : From socket_client to SSL_serve \n");
					SSL_write(ssl,str_data,sizeof(str_data));					
					char s[4];  
					memset(s,0,sizeof(s));                                                            //判断"quit"
					strncpy(s,str_data,sizeof(s));									
					if( 0 == strcmp("quit",s) )
					{
						printf("\nsocket_client方请求退出\n");
						break;
					}		
				}				
			}
			if( 0 != FD_ISSET(info_set->other_fd,&stExcpFds) )                                   //异常
			{
				printf("Error happened: %s\n",strerror(errno));
				break;
			}	
		}
	}	

	sleep(1);
	//关闭连接
	SSL_shutdown(ssl);
	SSL_free(ssl);
	SSL_CTX_free(client_ctx);

	//处理这个fd
	close(info_set->connect_fd);
	close(info_set->other_fd);
	return 0;

}

void ShowCerts(SSL * ssl)
{
	X509              *cert;             //基于X509创建的证书
	char              *line;

	cert = SSL_get_peer_certificate(ssl);
	if (cert != NULL) 
	{
		printf("打印证书信息如下:\n");
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

//创建普通server_socket作为server接收客户端的socket
int server_socket(info_p info_set,info_p info_set_ssl_client)
{
	info_set.connect_fd = -1;    //用于服务器端连接
	info_set.accept_fd  = -1;    //用于服务器端传递信息
	pthread_t             tid;

	info_set.connect_fd = socket(AF_INET,SOCK_STREAM,0);       //创建套接字，ipV4地址，可靠的面向连接的字符流，最后0是默认协议
	if(info_set.connect_fd < 0)
	{
		printf("Create socket failure: %s\n", strerror(errno));  //如果创建失败，打印错误信息
		return -1;
	}

	//端口复用，解决进程退出后一段时间地址被占用的问题
	int           opt = 1;
	setsockopt(info_set.connect_fd,SOL_SOCKET,SO_REUSEADDR,(const void *)&opt,sizeof(opt) );

	//服务器设置，设置server地址和端口号，用于接受任何client端
	memset(&info_set.sock_addr,0,sizeof(info_set.sock_addr));           //清空再设置ipV4地址
	info_set.sock_addr.sin_family = AF_INET;                            //设置为ipV4地址
	info_set.sock_addr.sin_port = htons(info_set.port);                 //端口号，主机字节序转网络字节序,short是16位
	info_set.sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);             //设置host主机地址，INADDR_ANY是指定为0.0.0.0的地址，表示不确定地址，或指所有地址
	if( bind(info_set.connect_fd, (struct sockaddr *)&info_set.sock_addr, sizeof(info_set.sock_addr)) < 0 )
	{
		printf("Bind socket failure: %s\n", strerror(errno));  //如果绑定失败，打印错误信息
		return -2;
	}
	//printf("Socket bind ok \n");

	printf("\nsocket服务器设置信息:\n");
	show_info(info_set);
	printf("\nsocket_SSL客户端设置信息:\n");
	show_info(info_set_ssl_client);

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
		printf("等待client连接\n");
		info_set.accept_fd = accept(info_set.connect_fd,NULL,NULL);
		//accept接受的参数必须经过listen和bind处理过，返回一个新的socket码
		//新的socket传送数据，旧的socket还能继续用accept接受请求
		//后面两个参数设置为NULL，那么会在系统连接时自动根据客户端信息填入
		if(info_set.accept_fd < 0)
		{
			printf("Accept socket failure: %s\n", strerror(errno));  //如果监听失败，打印错误信息
			return -1;  
		}
		//printf("Socket Accept fd[%d]\n",info_set.accept_fd);      //如果成功，返回fd

		//一连上,则启用多线程去负责转发		
		info_set_ssl_client.other_fd = info_set.accept_fd;
		pthread_create(&tid,NULL,SSL_pthreads,&info_set_ssl_client);
	}
	close(info_set.connect_fd);
	return 0;	
}

void *SSL_pthreads (void *argv)
{
	info_p             *info_set = NULL;
	info_p              info_copy;
	memset(&info_copy,0,sizeof(info_p));
	memcpy(&info_copy,(info_p *)argv,sizeof(info_p));
	info_set = &info_copy;
	client_SSL(info_set);

	return NULL;
}

int show_info(info_p info_set)
{
	printf("打印信息如下\n");
	printf("域名或者IP:%s \n",info_set.servaddr);
	printf("端口号    :%d \n",info_set.port);

	return 0;
}

info_p* set_info_client(void * info_p_set,int argc, char **argv)       //设置client_SSL
{ 
	info_p         * info_set = (info_p *) info_p_set;

	if(1 == argc)                                                        //默认
	{
		memcpy(info_set->servaddr,"127.0.0.1",sizeof(info_set->servaddr));
		info_set->port = (unsigned short)PORT_SSL;
	}   
	else if(4 == argc && !strcmp("C",argv[1]))                           //设置ip和port
	{ 
		memcpy(info_set->servaddr,argv[2],sizeof(argv[2]));
		info_set->port = (unsigned short)atoi(argv[3]);	
	}
	else if(6 == argc && !strcmp("C",argv[1]) && !strcmp("S",argv[4]))   //设置ip和port
	{
		memcpy(info_set->servaddr,argv[2],sizeof(argv[2]));
		info_set->port = (unsigned short)atoi(argv[3]);	  
	}
	else 
	{	
		return info_set;
	}

	return info_set;
}

info_p *set_info_server(void * info_p_set,int argc, char **argv)          //设置client
{
	info_p         * info_set = (info_p *) info_p_set;

	if(1 == argc)    
	{
		info_set->port = (unsigned short)PORT_SOCKET;
	}
	else if(3 == argc && !strcmp("S",argv[1]))                             //设置port
	{ 
		info_set->port = (unsigned short)atoi(argv[2]);	
	}
	else if(6 == argc && !strcmp("C",argv[1]) && !strcmp("S",argv[4]))     //设置port
	{
		info_set->port = (unsigned short)atoi(argv[5]);	
	}
	else
	{
		return info_set;
	}

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


