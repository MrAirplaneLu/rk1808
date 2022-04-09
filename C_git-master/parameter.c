/*********************************************************************************
 *      Copyright:  (C) 2018 yanhuan
 *                  All rights reserved.
 *
 *       Filename:  parameter.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(05/12/2018)
 *         Author:  Yujie <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "05/12/2018 01:50:04 PM"
 *                 
 ********************************************************************************/

#include <stdio.h>



int main (int argc, char **argv)
{
    int   i;

//  for(i = 0;i < argc;i++)
    for(i = 0;argv[i] != NULL;i++)
    {   
        printf("argv[%d]:%s\n",i,argv[i]);

    }

    return 0;
} /* ----- End of main() ----- */


