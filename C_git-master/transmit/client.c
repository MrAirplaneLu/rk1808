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
/*********************************************************************************
 *  client端显示格式:
 *  发送时:
 *        ip    :
 *        port  :
 *        send  ：
 *
 *  接收时:
 *        ip    :
 *        port  :
 *        recv  ：
 ********************************************************************************/

#include "client.h"

int creat_client(info_p info_set)
{
	int                 stReval;
	struct timeval      st_time;
	fd_set              stReadFds,stWriteFds,stExcpFds;
	info_set.connect_fd = -1;           	//客户端只需要一个套接字：用来连接服务器	

	info_set.connect_fd = socket(AF_INET,SOCK_STREAM,0);//创建连接套接字，ipV4地址，可靠的面向连接的字符流，最后0是默认协议
	if(info_set.connect_fd < 0)
	{
		printf("Create socket failure: %s\n", strerror(errno));  //如果创建失败，打印错误
		return -1;
	}
	//printf("Socket create fd[%d]\n",info_set.connect_fd);      //如果成功，返回connect_fd

	//连接服务器的准备,要连接的server地址和端口号
	memset(&info_set.sock_addr,0,sizeof(info_set.sock_addr));  //清空再设置ipV4地址
	info_set.sock_addr.sin_family = AF_INET;                   //设置为ipV4地址
	info_set.sock_addr.sin_port = htons(info_set.port);        //端口号，主机字节序转网络字节序,short是16位
	//BSD网络软件包含inet_addr和inet_ntoa和inet_aton，用于二进制地址和‘.’分十进制字符表示，只适用于ipV4地址，即addr需要存放32位地址
	//另外提供两个新函数：inet_ntop和inet_pton同时支持ipV4和ipV6,即addr需要存放128位地址

	inet_aton(info_set.servaddr,&info_set.sock_addr.sin_addr);	

	//连接server端
	if(connect(info_set.connect_fd,(struct sockaddr *)&info_set.sock_addr,sizeof(info_set.sock_addr)) < 0)
	{	
		printf("connect to server failure: %s\n", strerror(errno));
		return -1;
	}
	printf("可以直接输入发送的内容:\n");

	while(1)
	{
		int retval = 0;
		//清空输入、输出
		memset(info_set.str_recv,0,sizeof(info_set.str_recv));
		memset(info_set.str_send,0,sizeof(info_set.str_send));

		FD_ZERO(&stReadFds);
		FD_ZERO(&stWriteFds);
		FD_ZERO(&stExcpFds);

		FD_SET(info_set.connect_fd,&stReadFds);
		FD_SET(0,&stReadFds);
		FD_SET(info_set.connect_fd,&stWriteFds);
		FD_SET(info_set.connect_fd,&stExcpFds);

		st_time.tv_sec   = (time_t)  ST_TIME_S ;
		st_time.tv_usec  = (time_t)  ST_TIME_US;

		stReval = select(info_set.connect_fd + 1,&stReadFds,&stWriteFds,&stExcpFds,&st_time);
		usleep(1000);
		if(stReval > 0)
		{			
			if( 0 != FD_ISSET(info_set.connect_fd,&stReadFds) )                                   //从socket_server读
			{
				if( (retval = read(info_set.connect_fd,info_set.str_recv,sizeof(info_set.str_recv))) < 0 )			
				{
					printf("Read from socket failure: %s\n",strerror(errno));
					break;
				}		
				else if(retval > 0)
				{
					usleep(1000);     //适当延长接收时长
					printf("ip   :%s  \n",info_set.servaddr);
					printf("port :%d  \n",(int)info_set.port);
					printf("recv :%s\n\n",info_set.str_recv);
				}				
			}

			if( 0 != FD_ISSET(info_set.connect_fd,&stWriteFds) )                                //或者从标准输入读，然后转写
			{
				usleep(1000);       //适当延长接收时长
				char s[4];                                                                       //判断"quit"
				fgets(info_set.str_send,sizeof(info_set.str_send),stdin);
				if( strlen(info_set.str_send) > 1 )                                         //只有一个回车则不吸收
				{
					memset(s,0,sizeof(s));
					strncpy(s,info_set.str_send,sizeof(s));
					if( 0 == strcmp("quit",s) )
					{
						printf("\n己方请求退出\n");
						write(info_set.connect_fd,info_set.str_send,sizeof(info_set.str_send));
						sleep(1);
						break;
					}
					else if( strlen(info_set.str_send) )
					{
						write(info_set.connect_fd,info_set.str_send,sizeof(info_set.str_send));			
						printf("ip   :%s  \n",info_set.servaddr);
						printf("port :%d  \n",(int)info_set.port);
						printf("send :%s\n\n",info_set.str_send);
					}		
				}			
			}
			if( 0 != FD_ISSET(info_set.connect_fd,&stExcpFds) )                                //异常
			{
				printf("Error happened: %s\n",strerror(errno));
				break;
			}	      	
		}

	}
	close(info_set.connect_fd);	

	return 0;
}



info_p set_info(info_p info_set,int argc, char **argv)
{
	if(1 == argc)    
	{
		memcpy(info_set.servaddr,"127.0.0.1",sizeof(info_set.servaddr));
		info_set.port = (unsigned short)PORT_SOCKET;
	}
	else if(3 == argc)
	{
		if(!isnum(argv[2]))
		{
			printf("请检查端口号\n");
			exit(0);
		}	
		memcpy(info_set.servaddr,argv[1],sizeof(argv[1]));
		info_set.port = (unsigned short)atoi(argv[2]);	
	}
	else 
	{	
		printf("客户端正确格式如:");
		printf("%s 127.0.0.1 12000 \n",argv[0]);
		exit(0);
	}

	//清空发送、接收空间
	memset(info_set.str_send,0,sizeof(info_set.str_send));
	memset(info_set.str_recv,0,sizeof(info_set.str_recv));

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


