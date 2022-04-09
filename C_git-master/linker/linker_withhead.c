/*********************************************************************************
 *      Copyright:  (C) 2018 Yujie
 *                  All rights reserved.
 *
 *       Filename:  linker.c
 *    Description:  有头结点的链表
 *                 
 *        Version:  1.0.0(08/23/2018)
 *         Author:  yanhuan <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "08/23/2018 08:14:27 AM"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <malloc.h>


typedef  struct  node
{
    int          data;
    struct node *next;
}NODE_t,*NODE_p;


NODE_p   create_linker(void)
{
    NODE_p   s = (NODE_p)malloc(sizeof(NODE_p));
    s->data = 0x00;
    s->next = NULL;

    return s;
}


NODE_p   create_linker_Tail(NODE_p  Np)      //传入头指针
{
    int         length,n = 1;
    NODE_p      Np_now = Np;            //当前位置指针

    printf("Please input the length of linker you want to creat : \n");
    scanf("%d",&length);

    while(n <= length)
    {
        NODE_p   s = (NODE_p)malloc(sizeof(NODE_t));
        printf("Please input the %d num : ",n);
        scanf("%d",&s->data);

        s->next = NULL;
        Np_now->next = s;
        Np_now = s;
        n++;
    }

    printf("Input finished \n\n");

    return  Np;
}


NODE_p   create_linker_Head(NODE_p  Np) 
{
    int         length;

    printf("Please input the length of linker you want to creat : \n");
    scanf("%d",&length);

    while(length)
    {
        NODE_p   s = (NODE_p)malloc(sizeof(NODE_t));
        printf("Please input the NO.%d data : ",length);
        scanf("%d",&s->data);

        s->next  = Np->next;
        Np->next = s;
        length--;
    }

    printf("Input finished \n\n");

    return  Np;
}

NODE_p    insert_Node(NODE_p  Np,int  data,int  pos)
{
    int     i = 1;
    NODE_p  Np_now = Np->next;

    while(Np_now != NULL)
    {
        if(i == pos)
        {
            NODE_p   s = (NODE_p)malloc(sizeof(NODE_t));
            s->data = data;
            s->next = Np->next;
            Np->next = s;
            printf("Insert No.%d data is %d success\n",i,s->data);
            return  Np;
        }
        else
        {
            Np_now = Np_now->next;
            i++;
        }
    }

    printf("Insert error : Cannot find this position %d \n\n",pos);

    return  Np;
}

NODE_p    delete_Node(NODE_p  Np,int  pos)
{
    int    i = 1;
    NODE_p  Np_now = Np->next;
    NODE_p  p;

    while(Np_now->next != NULL)
    {
        if(i == pos-1)
        {
            p = Np_now->next;
            Np_now->next = Np_now->next->next;
            printf("Delete No.%d data is %d success\n",i,p->data);
            free(p);
            return  Np;
        }
        else
        {
            Np_now = Np_now->next;
            i++;
        }
    }

    printf("Delete error : Cannot find this position %d \n\n",pos);

    return  Np;

}

void      destory_linker(NODE_p  Np)
{
    NODE_p    Np_now;

    while(Np->next != NULL)
    {
        Np_now = Np;
        Np = Np->next;
        Np_now->next = NULL;
        free(Np_now);
    }

    printf("Destory linker finished \n\n");

}


void      travel(NODE_p  Np)
{
    NODE_p      Np_now = Np->next;     //当前位置指针
    int         n = 1;

    while(Np_now != NULL)
    {
        printf("The NO.%d data of linker is %d\n",n,Np_now->data);
        n++;
        Np_now = Np_now->next;
    }

    printf("Travel finished\n\n");
}



int main (int argc, char **argv)
{

    NODE_p        Np = create_linker();     //头指针，分配头结点

    create_linker_Head(Np);

    travel(Np);

    insert_Node(Np,3,1);

    travel(Np);

    delete_Node(Np,2);

    travel(Np);

    destory_linker(Np);

    travel(Np);

    return 0;
}



