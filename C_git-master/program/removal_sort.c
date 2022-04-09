/*********************************************************************************
 *      Copyright:  (C) 2018 yanhuan
 *                  All rights reserved.
 *
 *       Filename:  removal_sort.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(10/01/2018)
 *         Author:  Yujie <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "10/01/2018 04:15:25 PM"
 *                 
 ********************************************************************************/


#include <stdio.h>
#include <string.h>

//#define  DEBUG  

int   main()
{
    char    s[10000];
    memset(s,0,sizeof(s));
    fgets(s,sizeof(s),stdin);

    int   i=0,j=0,n=0,num=0;
    int   in[256]={0};

    char   *p=s;

    i=0;
    while(*p!='\0')                              //转换16进制整数 
    {
        if(*p>='0'&&*p<='9')
            in[i]=in[i]*16+*p-'0';
        else if(*p>='a'&&*p<='f')
            in[i]=in[i]*16+*p-'a'+10;
        else if(*p>='A'&&*p<='F')
            in[i]=in[i]*16+*p-'A'+10;

        if(*p==' ')
            i++;
        p++;
    }

    n=i+1;num=n;                                 //记录输入的个数 
#ifdef  DEBUG
    printf("\n");
    for(i=0;i<n;i++)
        printf("in[%d]=%x ",i,in[i]);
#endif

    for(i=0;i<n-1;i++)                           //排序 
        for(j=0;j<n-1-i;j++)
        {
            if(in[j]>in[j+1])
            {
                int   tmp=in[j+1];
                in[j+1]=in[j];
                in[j]=tmp;
            }
        }

#ifdef  DEBUG    
    printf("\n");
    for(i=0;i<n;i++)                    
        printf("in[%d]=%x ",i,in[i]);
#endif

    for(i=0;i<n-1;i++)                           //清除重复 
    {
        if(in[i]==in[i+1])
        {
            in[i]=0;
            num--;
        }    
    }

    for(i=0;i<n-1;i++)                           //排序 
        for(j=0;j<n-1-i;j++)
        {
            if(in[j]<in[j+1])
            {
                int   tmp=in[j+1];
                in[j+1]=in[j];
                in[j]=tmp;
            }
        }

#ifdef  DEBUG    
    printf("\n");
    for(i=0;i<num;i++)
        printf("in[%d]=%x ",i,in[i]);
    printf("\n");
#endif

    for(i=num-1;i>=0;i--)
    {
        printf("%x",in[i]);

        if(i!=0)
            printf(" ");
    }

    return   0;
}


