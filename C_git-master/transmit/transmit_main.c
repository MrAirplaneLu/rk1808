/*********************************************************************************
 *      Copyright:  (C) 2018 Yujie
 *                  All rights reserved.
 *
 *       Filename:  transmit_main.c
 *    Description:  Transmit between comport/socket and SSL  
 *                 
 *        Version:  1.0.0(07/30/2018)
 *         Author:  yanhuan <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "07/30/2018 10:52:59 PM"
 *                 
 ********************************************************************************/

#include "transmit.h"

int main (int argc, char **argv)
{
	info_p          info_sta_client;
	info_p          info_sta_server;

	if(1 == argc)                                //示例:./tranmit
	{
		set_info_client(&info_sta_client,argc,argv);
		set_info_server(&info_sta_server,argc,argv);
	}
	else if(3 == argc && !strcmp("S",argv[1]))   //示例:./tranmit S 12001
	{
		if(!isnum(argv[2]))
		{
			printf("请检查端口号\n");
			exit(0);
		}	
		set_info_client(&info_sta_client,argc,argv);
		set_info_server(&info_sta_server,argc,argv);
	}
	else if(4 == argc && !strcmp("C",argv[1]))   //示例:./tranmit C 127.0.0.1 12000
	{
		if(!isnum(argv[3]))
		{
			printf("请检查端口号\n");
			exit(0);
		}	
		set_info_client(&info_sta_client,argc,argv);
		set_info_server(&info_sta_server,argc,argv);
	}
	else if(6 == argc && !strcmp("C",argv[1]) && !strcmp("S",argv[4]))   //示例:./tranmit C 127.0.0.1 12000 S 12001
	{
		if(!isnum(argv[3]) || !isnum(argv[5]))
		{
			printf("请检查端口号\n");
			exit(0);
		}	
		set_info_client(&info_sta_client,argc,argv);
		set_info_server(&info_sta_server,argc,argv);
	}
	else
	{
		printf("正确格式:\n");
		printf("%s                          \n",argv[0]);
		printf("%s C 127.0.0.1 12000        \n",argv[0]);
		printf("%s S 12001                  \n",argv[0]);
		printf("%s C 127.0.0.1 12000 S 12001\n",argv[0]);
		exit(0);
	}	

#if   1
	printf("Cliet ip:%s\n",info_sta_client.servaddr);
	printf("S端口号 :%d\n",info_sta_server.port);
	printf("C端口号 :%d\n",info_sta_client.port);
#endif  

	server_socket(info_sta_server,info_sta_client);        //一个socket_server负责监听



	return 0;
}








