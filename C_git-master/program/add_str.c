/*********************************************************************************
 *      Copyright:  (C) 2018 yanhuan
 *                  All rights reserved.
 *
 *       Filename:  add_str.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(10/01/2018)
 *         Author:  Yujie <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "10/01/2018 04:29:29 PM"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>

char    *sum(const   char   *num1,const   char   *num2)
{
    printf("%s %s\n",num1,num2);
    char     s[128]={0};       
    char    *pn1=(char*)num1;        
    char    *pn2=(char*)num2;
    int        i=0,flag=0;

    while(*(pn1+1)!= '\0')              //移动到最后一位 
        pn1++;

    while(*(pn2+1) != '\0')
        pn2++;      

    while(1)      
    {       
        if(pn1>=num1 && pn2>=num2)           //2个都有数字
        {
            if(*pn1+*pn2-0x60+flag>10)                //和大于10
            {
                s[i]=*pn1+*pn2+flag-0x30-10;
                flag=1;
            }
            else
            {
                s[i]=*pn1+*pn2+flag-0x30;
                flag=0;
            }
        }
        else if(pn1>=num1 && pn2<num2)       //1个有数字
        {
            if(*pn1-0x30+flag>10)                     //和大于10
            {
                s[i]=*pn1+flag-10;
                flag=1;
            }
            else
            {
                s[i]=*pn1+flag;
                flag=0;
            }
        }
        else if(pn1<num1 && pn2>=num2)       //1个有数字
        {
            if(*pn2-0x30+flag>10)                     //和大于10
            {
                s[i]=*pn2+flag-10;
                flag=1;
            }
            else
            {
                s[i]=*pn2+flag;
                flag=0;
            }
        }
        else
        {
            break;
        }        

//        printf("s[i]=%c\n",s[i]);
        pn1--;pn2--;i++;
    }

    s[i]='\0';

    pn1=s;
    pn2=s+i-1;
    char     tmp;

    for(;pn1<=pn2;)                                     //逆序
    {
        tmp=*pn2;
        *pn2=*pn1;
        *pn1=tmp;
        pn1++;
        pn2--;
    }

    return   s;
}

int   main()
{  
    char    s[100]={'\0'};
    char   *p=s;

//    printf("%s\n",p);                      //这一句为什么不能删掉
    strcpy(s,sum("118866","66554422"));

    printf("%s\n",p);

    return  0;
}


