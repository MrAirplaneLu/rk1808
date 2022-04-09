/*********************************************************************************
 *      Copyright:  (C) 2018 Yujie
 *                  All rights reserved.
 *
 *       Filename:  linker_stack.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(08/26/2018)
 *         Author:  yanhuan <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "08/26/2018 11:18:00 PM"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

typedef  struct  node
{
    int          data;
    struct node *next;
}NODE_t,*NODE_p;

typedef  struct  stack    //先进后出,栈底不动
{
    struct node *top;
    struct node *bot;
}stack_t,*stack_p;


stack_p  create_linker_stack(stack_p  sta)        //以无头结点为基础
{
    int      length,n = 1;
    NODE_p   Np_now = NULL;
    sta->bot = NULL;
    sta->top = NULL;

    printf("Please input the init length of stack you want to creat : \n");
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
            sta->bot = Np_now;
            sta->top = Np_now;
        }
        else
        {
            Np_now->next = s;
            Np_now = s;
            sta->top = Np_now;
        }
        
        n++;
    }

    return   sta;
}


stack_p  insert_stack(stack_p  sta,int  data)           //只能从栈顶插入
{
    NODE_p   s = (NODE_p)malloc(sizeof(NODE_t));

    s->data = data;
    s->next = NULL;
   
    if(NULL == sta->top)
    {
        sta->top = s;
        sta->bot = s;
    }      
    else
    {
        sta->top->next = s;
        sta->top = s;
    }
    printf("Insert data:%d successful\n",s->data);

    return  sta;
}


stack_p  delete_stack(stack_p  sta)                    //只能从栈顶删除
{
    NODE_p   Np_now = NULL;

    if(sta->top == NULL)
    {
        printf("The stack has no element\n");
        return  sta;
    }

    Np_now = sta->bot;

    if(Np_now == sta->top)
    {
        sta->top = NULL;
        sta->bot = NULL;
        free(Np_now);
        Np_now->next = NULL;
        return  sta;
    }

    while(Np_now->next != sta->top)
    {
        Np_now = Np_now->next;
    }
    printf("Delete  data:%d successful\n",sta->top->data);
    free(sta->top);
    sta->top = Np_now;
    sta->top->next = NULL;

    return  sta;
}


stack_p   destory_stack(stack_p  sta)
{
    NODE_p   Np_now = sta->bot;

    while(Np_now != NULL)
    {
        sta->bot = sta->bot->next;
        free(Np_now);
        Np_now->next = NULL;
        Np_now = sta->bot;
    }

    sta->bot = NULL;
    sta->top = NULL;

    printf("Destory finished\n");

    return  sta;
}


void    travel(stack_p  sta)
{
    int           n = 1;
    NODE_p   Np_now = sta->bot;

    if(sta->top == NULL)
    {
        printf("The stack has no element\n");
        return ;
    }

    while(Np_now != NULL)
    {
        printf("The NO.%d data of stack is %d\n",n,Np_now->data);
        Np_now = Np_now->next;
        n++;
    }

    printf("Travel finished\n\n");

}


void   print_desc_main()
{
    printf("Please input the selection number you want:\n");
    printf("1   create  stack \n");
    printf("2   insert  stack  node \n");
    printf("3   delete  stack  node \n");
    printf("4   travel  stack \n");
    printf("5   destory stack \n");
    printf("6   exit \n");
}

void print_desc_insert()
{
    printf("what do you want to insert:\n");
}

int main (int argc, char **argv)
{
    int       i=0,data;
    stack_t   stack_n;
    stack_p   sta = (stack_p)&stack_n;
    sta->top = NULL;
    sta->bot = NULL;
//    sta = create_linker_stack(sta);
    
    while(1)
    {
        print_desc_main();
        scanf("%d",&i);
    
        if(isdigit(i) == 0)
            continue;
        
        switch(i)
        {
            case  1:
                if(sta->bot == NULL)
                {                    
                    sta = create_linker_stack(sta);
                }
                else
                    printf("The stack is exist\n");
                break;
            case   2:
                print_desc_insert();
                scanf("%d",&data);  
                sta = insert_stack(sta,data);
                break;
            case   3:
                sta = delete_stack(sta);
                break;
            case   4:
                travel(sta);
                break;
            case   5:
                sta = destory_stack(sta);
                break;
            case   6:
                sta = destory_stack(sta);
                printf("Quit \n");
                return  0;
                break;
            default:
                printf("Incorrect  input\n");
                print_desc_main(); 
                break;
        }
        
    }

    return 0;
}




