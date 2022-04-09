/*********************************************************************************
 *      Copyright:  (C) 2018 Yujie
 *                  All rights reserved.
 *
 *       Filename:  linker.c
 *    Description:  没有头结点的链表
 *                 
 *        Version:  1.0.0(08/23/2018)
 *         Author:  yanhuan <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "08/23/2018 08:14:27 AM"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>


typedef  struct  node
{
    int          data;
    struct node *next;
}NODE_t,*NODE_p;


NODE_p   create_linker_Tail(NODE_p  Np)      //传入头指针
{
    int         length,n = 1;
    NODE_p      Np_now = Np;            //当前位置指针

    printf("Please input the length of linker you want to creat : \n");
    scanf("%d",&length);

    while(n <= length)
    {
        NODE_p   s = (NODE_p)malloc(sizeof(NODE_t));
        printf("Please input the No.%d num : ",n);
        scanf("%d",&s->data);
        s->next = NULL;

        if(1 == n)
        {
            Np = s;
            Np_now = Np;
        }
        else
        {
            Np_now->next = s;
            Np_now = s;
        }
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

        s->next = Np;
        Np = s;    
        length--;
    }

    printf("Input finished \n\n");

    return  Np;
}

NODE_p    insert_Node(NODE_p  Np,int  data,int  pos)
{
    int     i = 1;
    NODE_p  Np_now = Np;

    while(Np_now != NULL)
    {
        if(1 == pos || i == pos-1)
        {
            NODE_p   s = (NODE_p)malloc(sizeof(NODE_t));
            s->data = data;

            if(1 == pos)
            {
                s->next = Np_now;
                Np_now = s;
                Np = Np_now; 
                printf("Insert No.%d data is %d success\n",i,s->data);
            }
            else
            {
                s->next = Np_now->next;
                Np_now->next = s; 
                printf("Insert No.%d data is %d success\n",i+1,s->data);
            }
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
    int     i = 1;
    NODE_p  Np_now = Np;
    NODE_p  p;

    while(Np_now != NULL)
    {
        if(i == pos-1)
        {
            p = Np_now->next;
            Np_now->next = Np_now->next->next;
            printf("Delete No.%d data is %d success\n",i+1,p->data);
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

NODE_p   destory_linker(NODE_p  Np)
{
    NODE_p    Np_now = Np;

    while(Np_now != NULL)
    {
        Np_now = Np;
        Np = Np->next;
        free(Np_now);
        Np_now->next = NULL;
        Np_now = NULL;
    }

    printf("Destory linker finished \n\n");

    return  Np_now;
}


void      travel(NODE_p  Np)
{
    NODE_p      Np_now = Np;     //当前位置指针
    int               n = 1;

    while(Np_now != NULL)
    {
        printf("The NO.%d data of linker is %d\n",n,Np_now->data);
        Np_now = Np_now->next;
        n++;
    }

    printf("Travel finished\n\n");
}


NODE_p     reverse1(NODE_p   Np)         //方法一：拆一个拼一个
{
    NODE_p     Np_now = Np; 

    if(Np_now == NULL)
    {
        printf("The linker is empty\n");
        return  Np_now;
    }

    NODE_p     p = Np->next;
    NODE_p     q = Np->next->next;

    while(p != NULL)
    {
        p->next = Np_now;
        Np_now = p;

        p = q;
        if(p != NULL)
            q = q->next;
    }

    printf("Reverser    finished\n");
    Np->next = NULL;
    Np = Np_now;
    return  Np_now;
}


NODE_p   findmiddle(NODE_p   Np)
{
    NODE_p   fast_p = Np;
    NODE_p   slow_p = Np;
    int           i = 1;

    if(Np == NULL)
    {
        printf("The linker is empty\n");
        return NULL;
    }

    while(fast_p != NULL)
    {
        if(fast_p->next != NULL && fast_p->next->next != NULL)
            fast_p = fast_p->next->next;
        else
            break;
        slow_p = slow_p->next;
        i++;
    }

    if(fast_p->next != NULL && fast_p->next->next == NULL)                //偶数个元素
    {
        printf("The middle Node are No.%d is %d and No.%d is %d\n\n",i,slow_p->data,i++,slow_p->next->data);
    }
    else                                                                 //奇数个元素
    {
        printf("The middle Node is No.%d is %d\n\n",i,slow_p->data);
    }

    return  slow_p;
}


void  print_desc_main()
{
    printf("Please input the selection number you want:\n");
    printf("1   create  linker \n");
    printf("2   insert  linker node \n");
    printf("3   delete  linker node \n");
    printf("4   traver  linker \n");
    printf("5   reverse linker \n");
    printf("6   findmiddle  node \n");
    printf("7   destory linker \n");
    printf("8   exit \n");
}


void  print_desc_create()
{
    printf("Create linker without head node:\n");
    printf("1 is Head insertion method or 2 is Tail insertion method \n");
}


void print_desc_insert()
{
    printf("how do you want to insert:\n");
    printf("ins(data,pos)\n");
}


void print_desc_delete()
{
    printf("how do you want to delect:\n");
    printf("del(pos)\n");
}



int main (int argc, char **argv)
{

#if    1  
    NODE_p        Np = NULL;     //头指针，不分配头结点
    int           i = 0,pos,data;
    char          buf[20],num[10];

    char         *p1=NULL,*p2=NULL;

    while(1)
    {
        print_desc_main();
        memset(buf,0,sizeof(buf));
        scanf("%d",&i);

        switch(i)
        {
            case  1:                                          //创建
                print_desc_create();               
                scanf("%d",&data);
                if(1 == data)
                    Np = create_linker_Head(Np);
                else if(2 == data)
                    Np = create_linker_Tail(Np);
                break;
            case  2:                                          //插入
                print_desc_insert();
                scanf("%s",buf);
                printf("buf : %s\n",buf);
                if(strstr(buf,"ins") != NULL)
                {
                    memset(num,0,sizeof(num));
                    p1 = strchr(buf,'(');
                    p2 = strchr(buf,',');
                    snprintf(num,p2-p1,"%s",p1+1);
                    data = atoi(num);
                    memset(num,0,sizeof(num));
                    p1 = p2;
                    p2 = strchr(buf,')');
                    snprintf(num,p2-p1,"%s",p1+1);
                    pos = atoi(num);
                }
                printf("pos = %d,data = %d\n",pos,data);
                Np = insert_Node(Np,data,pos); 
                break;
            case  3:                                          //删除
                print_desc_delete();
                scanf("%s",buf);
                printf("buf : %s\n",buf);
                if(strstr(buf,"del") != NULL)
                {
                    memset(num,0,sizeof(num));
                    p1 = strchr(buf,'(');
                    p2 = strchr(buf,')');
                    snprintf(num,p2-p1,"%s",p1+1);
                    pos = atoi(num);    
                }
                printf("pos = %d\n",pos);
                Np = delete_Node(Np,pos); 
                break;
            case  4:
                travel(Np);                                   //遍历
                break;
            case  5:
                Np = reverse1(Np);
                break;
            case  6:                                          //找到中间结点
                findmiddle(Np);
                break;
            case  7:                                          //销毁
                Np = destory_linker(Np);
                break;
            case  8:                                          //结束
                Np = destory_linker(Np);
                printf("Exit finished\n");
                return 0;        
            default:
                print_desc_main();    
                break;
        }       
    }
#endif

    return 0;
}



