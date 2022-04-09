/*********************************************************************************
 *      Copyright:  (C) 2018 Yujie
 *                  All rights reserved.
 *
 *       Filename:  webcam_client.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(08/08/2018)
 *         Author:  yanhuan <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "08/08/2018 02:43:35 PM"
 *                 
 ********************************************************************************/

#include <sys/socket.h>  //socketͷ�ļ�
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define  PORT            7999
#define  BUFFSIZE        64
#define  DOMAIN_MAX_LEN  128
#define  USER_NUM        5
#define  IP_NAME         "127.0.0.1"
#define  DEBUG           1

char   sign_ERR[4] = {0x2f,0x3f,0x0f,0x00};
char   sign_RIG[4] = {0x2f,0x3f,0x1f,0x00};

char   sign_ON [4] = {0x2f,0x3f,0x4f,0x00};
char   sign_OFF[4] = {0x2f,0x3f,0x5f,0x00};
char   sign_ASK[4] = {0x2f,0x3f,0x6f,0x00};

typedef  struct  info
{
    char                hostname[DOMAIN_MAX_LEN]; //��¼����
    char                servaddr[DOMAIN_MAX_LEN]; //��¼ip
    int                 connect_fd;               //���ڷ����������ӣ������ڿͻ��˴�����Ϣ
    int                 accept_fd;                //���ڷ������˴�����Ϣ
    struct sockaddr_in  sock_addr;      
    unsigned short      port;
    char                str_data[BUFFSIZE];
}info_p;

int  creat_client(info_p info_set);

int  main(int argc, char **argv)
{
    info_p           info_sta;

    memcpy(info_sta.servaddr,IP_NAME,sizeof(info_sta.servaddr));
    info_sta.port = PORT;
    creat_client(info_sta);
    return 0;	

}

int  creat_client(info_p info_set)
{
    info_set.connect_fd = -1;    //���ڿͻ�������
    int            reval = -1;

    //�����׽���
    info_set.connect_fd = socket(AF_INET,SOCK_STREAM,0);            //�����׽��֣�ipV4��ַ���ɿ����������ӵ��ַ��������0��Ĭ��Э��
    if(info_set.connect_fd < 0)
    {
        printf("Create socket failure: %s\n", strerror(errno));       //�������ʧ�ܣ���ӡ������Ϣ
        return -1;
    }
    printf("Socket create fd[%d]\n",info_set.connect_fd);           //����ɹ�������fd

    //�ͻ������ã�����client��ַ�Ͷ˿ں�
    memset(&info_set.sock_addr,0,sizeof(info_set.sock_addr));       //���������ipV4��ַ
    info_set.sock_addr.sin_family = AF_INET;                        //����ΪipV4��ַ
    info_set.sock_addr.sin_port = htons(info_set.port);             //�˿ںţ������ֽ���ת�����ֽ���,short��16λ

    //�˿ڸ��ã���������˳���һ��ʱ���ַ��ռ�õ�����
    int           opt = 1;
    setsockopt(info_set.connect_fd,SOL_SOCKET,SO_REUSEADDR,(const void *)&opt,sizeof(opt) );

    //����server
    while( connect(info_set.connect_fd, (struct sockaddr *)&info_set.sock_addr, sizeof(info_set.sock_addr)) < 0)
    {
        printf("Connect to server failure: %s\n", strerror(errno));
        printf("Try to connect again\n");
        sleep(1);
        continue;
    }
    printf("Connect to server success\n");

    printf("send username && password now\n");
    char        login_info[42];
    memset(login_info,0,sizeof(login_info));
    login_info[0] = 0x2f;
    login_info[1] = 0x3f;
    memcpy(login_info+2,"admin:admin",sizeof("admin:admin"));
    send(info_set.connect_fd,login_info,sizeof(login_info),0);

    memset(info_set.str_data,0,sizeof(info_set.str_data));
    if((reval = recv(info_set.connect_fd,info_set.str_data,sizeof(info_set.str_data),0)) == -1)
    {
        printf("Recv error:%s\n",strerror(errno));
        return -1;
    }	
    else if(reval == 0)
    {
        printf("Lost server connection\n");
        return -1;
    }	
    else
    {
        if(info_set.str_data[0] == 0x2f && info_set.str_data[1] == 0x3f && info_set.str_data[2] == 0x1f)
            printf("Login successfully\n");
        else if(info_set.str_data[0] == 0x2f && info_set.str_data[1] == 0x3f && info_set.str_data[2] == 0x0f)
            printf("Login failed\n");				
    }	
    //����
    printf("traverse:\n");
    int    j = 0;
    for(j=0;j<strlen(info_set.str_data);j++)
    {
        printf("0x%02x  ",info_set.str_data[j]);
    }
    printf("\n");

    while(1)
    {
        reval = -1;
        printf("ASK    :2\n");
        printf("Trun ON:1\n");
        printf("Trun ON:0\n");
        memset(info_set.str_data,0,sizeof(info_set.str_data));

        fgets(info_set.str_data,sizeof(info_set.str_data),stdin);
        if(strlen(info_set.str_data) > 1)
        {
            if(info_set.str_data[0] == '1')
            {
                printf("You want to trun ON\n");
                usleep(1000);
                send(info_set.connect_fd,sign_ON,sizeof(sign_ON),0);
            }							
            else if(info_set.str_data[0] == '0')
            {
                printf("You want to trun OFF\n");
                usleep(1000);
                send(info_set.connect_fd,sign_OFF,sizeof(sign_OFF),0);
            }	
            else if(info_set.str_data[0] == '2')
            {
                printf("You want to ASK\n");
                usleep(1000);
                send(info_set.connect_fd,sign_ASK,sizeof(sign_ASK),0);
            }	
            else
            {
                printf("What do you want to do ?\n");
                printf("\nRestart\n");
                continue;
            }						
        }
        else
        {
            continue;
        }	

        if((reval = recv(info_set.connect_fd,info_set.str_data,sizeof(info_set.str_data),0)) == -1)
        {
            printf("Recv error\n");
            break;	
        }	
        else if(reval == 0)
        {
            printf("Other is disconnetde\n");
            break;			
        }
        else
        {
            printf("This is recv value\n");
            printf("Recv data:%s\n",info_set.str_data);
            printf("Recv data:0x%02x 0x%02x 0x%02x 0x%02x\n",info_set.str_data[0],info_set.str_data[1],info_set.str_data[2],info_set.str_data[3]);	
            if(info_set.str_data[2] == 0x4f)
                printf("The web_camera is ON \n");
            if(info_set.str_data[2] == 0x5f)
                printf("The web_camera is OFF\n");
        }	

        printf("\nRestart\n");					
    }

    close(info_set.connect_fd);	

    return 0;
}


