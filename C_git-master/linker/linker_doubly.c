/*********************************************************************************
 *      Copyright:  (C) 2018 Yujie
 *                  All rights reserved.
 *
 *       Filename:  linker_doubly.c
 *    Description:  不带头结点的双向链表
 *                 
 *        Version:  1.0.0(08/27/2018)
 *         Author:  yanhuan <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "08/27/2018 01:58:42 PM"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <malloc.h>

typedef  struct  linker
{
    int              data;
    struct  linker  *prev;
    struct  linker  *next;
}linker_t,*linker_p;


linker_p   create_linker_Tail(linker_p  Head)
{
    int         length,n = 1;
    linker_p    Np_now = Head;
    Np_now->prev = NULL;
    Np_now->next = NULL;

    printf("Please input the init length of linker you want to creat : ");
    scanf("%d",&length);

    while(n <= length)
    {
        linker_p s = (linker_p)malloc(sizeof(linker_t));
        printf("Please input the NO.%d data : ",n);
        scanf("%d",&s->data);
        s->prev = NULL;
        s->next = NULL;

        if(1 == n)
        {
            Head   = s;
            Np_now = s;
        }
        else
        {
            Np_now->next = s;
            s->prev = Np_now;
            Np_now = s;
        }
        n++;
    }

    printf("Input finished \n\n");

    return  Head;
}



linker_p   create_linker_Head(linker_p  Head)
{
    int         length,t_flag = 1;
    Head->prev = NULL;
    Head->next = NULL;

    printf("Please input the init length of linker you want to creat : ");
    scanf("%d",&length);

    while(length)
    {
        linker_p s = (linker_p)malloc(sizeof(linker_t));
        printf("Please input the NO.%d data : ",length);
        scanf("%d",&s->data);
        s->prev = NULL;
        s->next = NULL;

        if(t_flag)
        {
            Head   = s;
            t_flag = 0;
        }
        else
        {
            s->next = Head;
            Head->prev = s;
            Head = s;
        }
        length--;
    }

    printf("Input finished \n\n");

    return  Head;
}


linker_p   insert_Node(linker_p  Head,int  data,int  pos)
{
    int       i = 1;
    linker_p  Np_now = Head;

    while(Np_now != NULL)
    {
        if(1 == pos || i == pos-1)
        {
            linker_p  s = (linker_p)malloc(sizeof(linker_t));
            s->data = data;

            if(1 == pos)
            {
                s->next = Head;
                Head->prev = s;
                Head = s;
                printf("Insert No.%d data is %d success\n",i,s->data);
            }
            else
            {
                s->next = Np_now->next;
                Np_now->next->prev = s;
                Np_now->next = s;
                s->prev = Np_now;
                printf("Insert No.%d data is %d success\n",i+1,s->data);
            }
            return  Head;
        }
        Np_now = Np_now->next;
    }

    printf("Insert error : Cannot find this position %d \n\n",pos);

    return  Head;
}


linker_p   delete_Node(linker_p  Head,int   pos)
{
    int        i = 1;
    linker_p   Np_now = Head;
    linker_p   p;
   
    if(Head->prev == NULL && Head->next == NULL)
    {
        printf("Delete No.%d data is %d success\n",i,Head->data);
        free(Np_now);
        Head->prev = NULL;
        Head->next = NULL;
        return  Head;    
    }

    while(Np_now != NULL)
    {
        if(i == pos-1)
        {
            p = Np_now->next;
            Np_now->next = Np_now->next->next;
            Np_now->next->prev = Np_now;
            printf("Delete No.%d data is %d success\n",i+1,p->data);
            free(p);
            return  Head;
        }
        else
        {
            Np_now = Np_now->next;
            i++;
        }
    }

    printf("Delete error : Cannot find this position %d \n\n",pos);

    return   Head;
}




void    travel(linker_p  Head)
{
    linker_p    Np_now = Head;
    int               n = 1;

    while(Np_now != NULL)
    {
        printf("The NO.%d data of linker is %d\n",n,Np_now->data);
        Np_now = Np_now->next;
        n++;
    }

    printf("Travel finished\n\n");
}


int main (int argc, char **argv)
{
    linker_t   linker_n;
    linker_p   Head = (linker_p)&linker_n;

    Head = create_linker_Head(Head);

    travel(Head);

    Head = insert_Node(Head,100,1);
    travel(Head);

    Head = insert_Node(Head,99,2);
    travel(Head);

    Head = delete_Node(Head,3);
    travel(Head);

    Head = delete_Node(Head,1);
    travel(Head);
    return 0;
}



