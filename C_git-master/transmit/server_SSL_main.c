/*********************************************************************************
 *      Copyright:  (C) 2018 Yujie
 *                  All rights reserved.
 *
 *       Filename:  server_SSL_main.c
 *    Description:  server_SSL_main.c
 *                 
 *        Version:  1.0.0(04/21/2018)
 *         Author:  yanhuan <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "04/21/2018 09:21:24 PM"
 *                 
 ********************************************************************************/

#include "server_SSL.h"

int main (int argc, char **argv)
{
	info_p           info_sta;
	set_info(&info_sta,argc,argv);
	creat_server_SSL(info_sta);  

	return 0;  
} 



