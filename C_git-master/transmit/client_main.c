/*********************************************************************************
 *      Copyright:  (C) 2018 Yujie
 *                  All rights reserved.
 *
 *       Filename:  client_main.c
 *    Description:  client_main.c
 *                 
 *        Version:  1.0.0(04/26/2018)
 *         Author:  yanhuan <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "04/26/2018 07:16:03 PM"
 *                 
 ********************************************************************************/

#include "client.h"

int main (int argc, char **argv)
{
	info_p           info_sta;
	info_sta = set_info(info_sta,argc,argv);
	creat_client(info_sta);

	return 0;
}













