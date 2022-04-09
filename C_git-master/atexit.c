/*********************************************************************************
 *      Copyright:  (C) 2018 yanhuan
 *                  All rights reserved.
 *
 *       Filename:  atexit.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(05/12/2018)
 *         Author:  Yujie <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "05/12/2018 01:28:15 PM"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void my_exit1(void);
void my_exit2(void);


int main (int argc, char **argv)
{

    if(atexit(my_exit2) != 0)
        printf("can't register my_exit2");  //反向注册exit1

    if(atexit(my_exit1) != 0)
        printf("can't register my_exit1");  //反向注册exit2

    if(atexit(my_exit1) != 0)
        printf("can't register my_exit1");  //再一次反向注册exit2

    return 0;
} /* ----- End of main() ----- */


/**************************************************************************************
 *  Description:
 *   Input Args:
 *  Output Args:
 * Return Value:
 *************************************************************************************/
void my_exit1 (void)
{
    printf("反向注册函数exit1\n");
} /* ----- End of my_exit1()  ----- */


void my_exit2 (void)
{
    printf("反向注册函数exit2\n");
} /* ----- End of my_exit1()  ----- */

/* 
 *Output:
 向注册函数exit1
 反向注册函数exit1
 反向注册函数exit2
*/


