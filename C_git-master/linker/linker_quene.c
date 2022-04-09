/*********************************************************************************
 *      Copyright:  (C) 2018 Yujie
 *                  All rights reserved.
 *
 *       Filename:  linker_quene.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(08/27/2018)
 *         Author:  yanhuan <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "08/27/2018 12:47:42 PM"
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

typedef  struct  quene    //先进后出
{
    struct node *front;   //队首
    struct node *rear;    //队尾
}quene_t,*quene_p;


quene_p   create_linker_quene(quene_p  que)    //以无头结点为基础
{
    int     length,n = 1;
    NODE_p   Np_now = NULL;
    que->front = NULL;
    que->rear  = NULL;

    printf("Please input the init length of quene you want to creat : \n");
    scanf("%d",&length);

    while(n <= length)
    {
        NODE_p   s = (NODE_p)malloc(sizeof(NODE_t));
        printf("Please input the No.%d num : ",n);
        scanf("%d",&s->data);
        s->next = NULL;

        if(1 == n)
        {
            Np_now = s;
            que->front = Np_now;
            que->rear  = Np_now;
        }
        else
        {
            Np_now->next = s;
            Np_now = s;
            que->rear  = Np_now;
        }
        n++;
    }

    return  que;
}


quene_p   insert_quene(quene_p  que,int  data)        //只能从队尾插入
{
    NODE_p   s = (NODE_p)malloc(sizeof(NODE_t));

    s->data = data;
    s->next = NULL;

    if(NULL == que->front)
    {
        que->front = s;
        que->rear =  s;
    }
    else
    {
        que->rear->next = s;
        que->rear = s;
    }
    printf("Insert data:%d successful\n",s->data);

    return  que;
}


quene_p  delete_quene(quene_p   que)
{
    NODE_p   Np_now = que->front;

    if(NULL == Np_now)
    {
        printf("The quene has no element\n");
        return  que;  
    }

    if(Np_now == que->rear)
    {
        que->front = NULL;
        que->rear  = NULL;
        printf("Delete data:%d successful\n",Np_now->data);
        return  que;
    }

    que->front = que->front->next;
    printf("Delete data:%d successful\n",Np_now->data);
    free(Np_now);

    return  que;
}


quene_p   destory_quene(quene_p   que)
{
    NODE_p   Np_now = que->front;

    while(Np_now != NULL)
    {
        que->front = que->front->next;
        free(Np_now);
        Np_now->next = NULL;
        Np_now = que->front;
    }

    que->front = NULL;
    que->rear  = NULL;

    printf("Destory finished\n");

    return  que;
}


void    travel(quene_p  que)
{
    int           n = 1;
    NODE_p   Np_now = que->front;

    if(Np_now == NULL)
    {
        printf("The quene has no element\n");
        return ;
    }

    while(Np_now != NULL)
    {
        printf("The NO.%d data of quene is %d\n",n,Np_now->data);
        Np_now = Np_now->next;
        n++;
    }

    printf("Travel finished\n\n");
}


void   print_desc_main()
{
    printf("Please input the selection number you want:\n");
    printf("1   create  quene \n");
    printf("2   insert  quene  node \n");
    printf("3   delete  quene  node \n");
    printf("4   travel  quene \n");
    printf("5   destory quene \n");
    printf("6   exit \n");
}


void print_desc_insert()
{
    printf("what do you want to insert:\n");
}





int main (int argc, char **argv)
{
    int       i=0,data;
    quene_t   quene_n;
    quene_p   que = (quene_p)&quene_n;
    que->front = NULL;
    que->rear  = NULL;    

    //    que = create_linker_quene(que);

    while(1)
    {
        print_desc_main();
        scanf("%d",&i);

        switch(i)
        {
            case   1:
                if(que->front == NULL)
                {
                    que = create_linker_quene(que);
                }
                else
                    printf("The quene is exist\n");
                break;
            case   2:
                print_desc_insert();
                scanf("%d",&data);  
                que = insert_quene(que,data);
                break;
            case   3:
                que = delete_quene(que);
                break;
                break;
            case   4:
                travel(que);
                break;
            case   5:
                que = destory_quene(que);
                break;
            case   6:
                que = destory_quene(que);
                printf("Quit \n");
                return  0;
            default:
                printf("Incorrect  input\n");
                print_desc_main(); 
                break;
        }
    }

    return 0;
}



