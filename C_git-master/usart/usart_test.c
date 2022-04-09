#include "usart.h"
#include<stdio.h>      /*��׼�����������*/    
#include<stdlib.h>     /*��׼�����ⶨ��*/    
#include<unistd.h>     /*Unix ��׼��������*/    
#include<sys/types.h>     
#include<sys/stat.h>       
#include<fcntl.h>      /*�ļ����ƶ���*/    
#include<termios.h>    /*PPSIX �ն˿��ƶ���*/    
#include<errno.h>      /*����Ŷ���*/    
#include<string.h>


int main(int argc, char **argv)    
{
	  int fd = -1;           //�ļ����������ȶ���һ��������޹ص�ֵ����ֹfdΪ����ֵ���³����bug    
    int err;               //���ص��ú�����״̬    
    int len;                            
    int i;    
    char rcv_buf[256];             
    char send_buf[256];
    if(argc != 3)    
    {    
        printf("Usage: %s /dev/ttySn 1(send data)/1 (receive data) \n",argv[0]);
        printf("open failure : %s\n", strerror(errno));
    
        return FALSE;    
    }    
     fd = UART0_Open(fd,argv[1]); //�򿪴��ڣ������ļ�������   
     // fd=open("dev/ttyS1", O_RDWR);
    //printf("fd= \n",fd);
     do  
    {    
        err = UART0_Init(fd,115200,0,8,1,'N');    
        printf("Set Port Exactly!\n"); 
        sleep(1);   
    }while(FALSE == err || FALSE == fd);    
       
    if(0 == strcmp(argv[2],"0"))    //��������pc�������ݵ�ģʽ
    {   
        fgets(send_buf,256,stdin);   //�������ݣ���󲻳���40�ֽڣ�fgets�����ջس���������pc�յ������ݾ����Զ�����     
        for(i = 0;i < 10;i++)    
        {    
            len = UART0_Send(fd,send_buf,40);    
            if(len > 0)    
                printf(" %d time send %d data successful\n",i,len);    
            else    
                printf("send data failed!\n");    
                              
            sleep(1);    
        }    
        UART0_Close(fd);                 
    }    
    else                            //�������յ�pc���͵����ݵ�ģʽ                 
    {                                          
        while (1) //ѭ����ȡ����    
        {   
            len = UART0_Recv(fd, rcv_buf,sizeof(rcv_buf));    
            if(len > 0)    
            {    
                rcv_buf[len] = '\0';    
                printf("receive data is %s\n",rcv_buf);    
            }    
            else    
            {    
                printf("cannot receive data\n");    
            }    
            sleep(1);    
        }                
        UART0_Close(fd);     
    }    
}    
