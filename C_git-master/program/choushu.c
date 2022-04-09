/*********************************************************************************
 *      Copyright:  (C) 2018 yanhuan
 *                  All rights reserved.
 *
 *       Filename:  choushu.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(10/01/2018)
 *         Author:  Yujie <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "10/01/2018 04:25:12 PM"
 *                 
 ********************************************************************************/

#include <stdio.h>

int  main()
{
    int   n;
    int   i,x=2;        //auto  add

    int   count=1;

    scanf("%d",&n);     //input  N

    if(1==n)
    {
        printf("1");
        return  0;
    }

    while(1)
    {
        i = x;
        do
        {
            if(i%2==0)               //2
            {
                i=i/2;
            }
            else  if(i%3==0)         //3
            {
                i=i/3;
            }
            else  if(i%5==0)         //5
            {
                i=i/5;
            }
            else  if(i==1)           //choushu
            {
                count++;
                break;
            }
            else
            {
                break;
            }
        }while(1);

        if(count == n)
        {
            printf("%d",x);
            return  0;
        }

        x++;
    }

    return  0;
}


