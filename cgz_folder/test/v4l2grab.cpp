/*
 * @Description: 
 * @Version: 
 * @Autor: guanzhou
 * @email: guanzhou.cn@gmail.com
 * @Date: 2022-04-08 21:08:53
 * @LastEditors: guanzhou
 * @LastEditTime: 2022-04-09 19:12:06
 */
#include "v4l2grab.h"

#include "usart.hpp"


#include <pthread.h> 

//串口相关的头文件    
#include<stdio.h>      /*标准输入输出定义*/    
#include<stdlib.h>     /*标准函数库定义*/    
#include<unistd.h>     /*Unix 标准函数定义*/    
#include<sys/types.h>     
#include<sys/stat.h>       
#include<fcntl.h>      /*文件控制定义*/    
#include<termios.h>    /*PPSIX 终端控制定义*/    
#include<errno.h>      /*错误号定义*/    
#include<string.h>    



#include <iostream>
using namespace std;
using namespace cv;


//打开串口
int open_port(void)
{
    int fd;

    fd=open("/dev/ttyS0",O_RDWR | O_NOCTTY | O_NONBLOCK);//O_NONBLOCK设置为非阻塞模式，在read时不会阻塞住，在读的时候将read放在while循环中，下一节篇文档将详细讲解阻塞和非阻塞
//    printf("fd=%d\n",fd);

    if(fd==-1)
    {
        perror("Can't Open SerialPort");
    }else
    {
        printf("succ!\n");
    }
    

    return fd;
}


void* read_thread_func(void* args)
{
    char rcv_buf[256];
    int fd = *(int*)args;
    while (1) //循环读取数据    
    {   
        int len = UART0_Recv(fd, rcv_buf,sizeof(rcv_buf));    
        if(len > 0)    
        {    
            rcv_buf[len] = '\0';    
            printf("receive data is %s\n",rcv_buf);    
        }    
        else    
        {    
            // printf("cannot receive data\n");    
        }    
        sleep(1);    
    }                
    UART0_Close(fd);     
}


void* write_thread_func(void* args)
{
    printf("======================================\n");
    int fd = *(int*)args;
    char send_buf[256] = "1234567890";
    // fgets(send_buf,256,stdin);   //输入内容，最大不超过40字节，fgets能吸收回车符，这样pc收到的数据就能自动换行     
    for(int i = 0;i < INT_MAX;i++)    
    {    
        int len = UART0_Send(fd,send_buf,40);    
        if(len > 0)    
            printf(" %d time send %d data successful\n",i,len);    
        else    
            printf("send data failed!\n");    
                            
        sleep(1);    
    }    
    UART0_Close(fd);
}

int main(int argc, char **argv) 
{
	int fd = -1;           //文件描述符，先定义一个与程序无关的值，防止fd为任意值导致程序出bug    
    int err;               //返回调用函数的状态    
    int len;                            
    int i;    
    // char rcv_buf[256];             
    // char send_buf[256];
    if(argc != 3)    
    {    
        printf("Usage: %s /dev/ttySn 1(send data)/1 (receive data) \n",argv[0]);
        printf("open failure : %s\n", strerror(errno));
    
        return FALSE;    
    }    
     fd = UART0_Open(fd,argv[1]); //打开串口，返回文件描述符   
     // fd=open("dev/ttyS1", O_RDWR);
    //printf("fd= \n",fd);
     do  
    {    
        err = UART0_Init(fd,1500000,0,8,1,'N');    
        printf("Set Port Exactly!\n"); 
        sleep(1);   
    }while(FALSE == err || FALSE == fd);    
    



    pthread_t read_thread;
    pthread_t write_thread;

    pthread_create(&read_thread, NULL, read_thread_func, &fd);
    pthread_create(&write_thread, NULL, write_thread_func, &fd);



    // if(0 == strcmp(argv[2],"0"))    //开发板向pc发送数据的模式
    // {   
    //     fgets(send_buf,256,stdin);   //输入内容，最大不超过40字节，fgets能吸收回车符，这样pc收到的数据就能自动换行     
    //     for(i = 0;i < 10;i++)    
    //     {    
    //         len = UART0_Send(fd,send_buf,40);    
    //         if(len > 0)    
    //             printf(" %d time send %d data successful\n",i,len);    
    //         else    
    //             printf("send data failed!\n");    
                              
    //         sleep(1);    
    //     }    
    //     UART0_Close(fd);                 
    // }    
    // else                            //开发板收到pc发送的数据的模式                 
    // {                                          
    //     while (1) //循环读取数据    
    //     {   
    //         len = UART0_Recv(fd, rcv_buf,sizeof(rcv_buf));    
    //         if(len > 0)    
    //         {    
    //             rcv_buf[len] = '\0';    
    //             printf("receive data is %s\n",rcv_buf);    
    //         }    
    //         else    
    //         {    
    //             printf("cannot receive data\n");    
    //         }    
    //         sleep(1);    
    //     }                
    //     UART0_Close(fd);     
    // }    


    while(true)
    {
        usleep(1000);
    }




    return 0;


	cout << "place input a num\n";
	int a;
	cin >> a;
	cout << testFunc(a) << endl;
	// char *img_path = "100x100.jpg";
	// cv::Mat test = imread("./0.jpg");

	
    
    return 0;
}
