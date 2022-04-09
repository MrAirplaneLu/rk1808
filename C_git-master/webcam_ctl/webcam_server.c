/*********************************************************************************
 *      Copyright:  (C) 2018 Yujie
 *                  All rights reserved.
 *
 *       Filename:  webcam_server.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(08/07/2018)
 *         Author:  yanhuan <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "08/07/2018 12:24:18 PM"
 *                 
 ********************************************************************************/
/********************************************************************************
 *Tag     : 0x2f 0x3f 
 *username: 20bytes
 *password: 20bytes
 *request : 4f is turn ON 
 *          5f is turn OFF
 *          6f is ASK
 *return  : 0f is verify failed
 *          1f is verify successed
 *          4f is ON
 *          5f is OFF 
 ********************************************************************************/

#include <sys/socket.h>  //socket头文件
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  
#include <pthread.h>
#include <sys/select.h>    //select()复用头文件
#include <sys/resource.h>  //setrlimit()解除资源限制头文件
#include <sys/time.h>
#include <netdb.h>        //getaddrinfo()域名解析函数头文件

#define  PORT            7999
#define  BUFFSIZE        64
#define  DOMAIN_MAX_LEN  128
#define  USER_NUM        5
#define  PROCESS_NAME    mjpg_streamer 
#define  IP_NAME         "192.168.0.5"
#define  DEBUG           1

char   sign_ERR[4] = {0x2f,0x3f,0x0f,0x00};
char   sign_RIG[4] = {0x2f,0x3f,0x1f,0x00};

char   sign_ON [4] = {0x2f,0x3f,0x4f,0x00};
char   sign_OFF[4] = {0x2f,0x3f,0x5f,0x00};

char   username[USER_NUM][20] = {"yanhuan","admin","root"};
char   password[USER_NUM][20] = {"mini","admin","yanhuan"};

typedef  struct  info
{
    char                hostname[DOMAIN_MAX_LEN]; //记录域名
    char                servaddr[DOMAIN_MAX_LEN]; //记录ip
    int                 connect_fd;               //用于服务器端连接，或用于客户端传递信息
    int                 accept_fd;                //用于服务器端传递信息
    struct sockaddr_in  sock_addr;      
    unsigned short      port;
    char                str_data[BUFFSIZE];
}info_p;

int  creat_server(info_p info_set);
int  *server_pthreads(void  *argv);


int  main(int argc, char **argv)
{
    info_p           info_sta;
    info_sta.port = PORT;
    creat_server(info_sta);
    return 0;
}


int  creat_server(info_p info_set)
{
    info_set.connect_fd = -1;       //用于服务器端连接
    info_set.accept_fd  = -1;       //用于服务器端传递信息
    pthread_t             tid;      //多线程

    //创建套接字
    info_set.connect_fd = socket(AF_INET,SOCK_STREAM,0);            //创建套接字，ipV4地址，可靠的面向连接的字符流，最后0是默认协议
    if(info_set.connect_fd < 0)
    {
        printf("Create socket failure: %s\n", strerror(errno));       //如果创建失败，打印错误信息
        return -1;
    }
    //printf("Socket create fd[%d]\n",info_set.connect_fd);           //如果成功，返回fd

    //服务器设置，设置server地址和端口号，用于接受任何client端
    memset(&info_set.sock_addr,0,sizeof(info_set.sock_addr));       //清空再设置ipV4地址
    info_set.sock_addr.sin_family = AF_INET;                        //设置为ipV4地址
    info_set.sock_addr.sin_port = htons(info_set.port);             //端口号，主机字节序转网络字节序,short是16位

    info_set.sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);    

    //端口复用，解决进程退出后一段时间地址被占用的问题
    int           opt = 1;
    setsockopt(info_set.connect_fd,SOL_SOCKET,SO_REUSEADDR,(const void *)&opt,sizeof(opt) );     

    //绑定ip和端口号
    if( bind(info_set.connect_fd, (struct sockaddr *)&info_set.sock_addr, sizeof(info_set.sock_addr)) < 0 )
    {
        printf("Bind socket failure: %s\n", strerror(errno));         //如果绑定失败，打印错误信息
        return -1;
    }

#if  DEBUG
    memset(info_set.servaddr,0,sizeof(info_set.servaddr));
    if( (inet_ntop(AF_INET,(struct sockaddr *)&info_set.sock_addr,info_set.servaddr,sizeof(info_set.servaddr))) != NULL)
    {
        printf("Query address failure：%s\n",strerror(errno));
    }	

    printf("Server info：\n");
    printf("    ip    ：%s\n",info_set.servaddr);
    printf("    port  ：%d\n",info_set.port);	
#endif    

    printf("Socket_server bind ok \n");

    //优化最大文件描述符
    struct  rlimit  rt;
    rt.rlim_cur = 65000 ;	  //软件上限
    rt.rlim_max = 65000 ;     //硬件上限
    if(setrlimit(RLIMIT_NOFILE,&rt) == 0)
    {
        printf("Set up the maximum file descriptor：%d\n",(int)rt.rlim_cur);
    }	
    else
    {
        printf("Set up the maximum file descriptor failure: %s\n", strerror(errno));
        return -1;
    }	

    //设置排队等待连接数
    if(listen(info_set.connect_fd,255) != 0)
    {
        printf("Listen socket failure：%s\n",strerror(errno));
    }	

    while(1)		      	//接受连接加入
    {
        printf("Wait accept fd[%d]\n",info_set.connect_fd);

        info_set.accept_fd = accept(info_set.connect_fd,NULL,NULL);
        if(info_set.accept_fd < 0)
        {
            printf("Accept client failure: %s\n", strerror(errno)); 
            close(info_set.accept_fd);
        }
        printf("Accept client fd[%d] success\n",info_set.accept_fd);

        pthread_create(&tid,NULL,(void *)server_pthreads,&info_set);     //线程去处理接入的client	
        printf("\n");
    }

    close(info_set.connect_fd);

    return 0;
}


int  *server_pthreads(void  *argv)
{	
    usleep(1000);
    printf("Processing login information\n");

    info_p              info_A;                
    info_p             *info_set = NULL;
    char                name[20];
    char                pawd[20];
    int                 i;
    FILE               *fp;
    char               *p = NULL;

    memset(&info_A,0,sizeof(info_p));
    memcpy(&info_A,(info_p *)argv,sizeof(info_p));
    info_set = &info_A;

    while(1)                //处理登录信息
    {
        int 			 retval = 0;
        memset(info_set->str_data,0,sizeof(info_set->str_data));
        if((retval = recv(info_set->accept_fd,info_set->str_data,sizeof(info_set->str_data),0)) == -1)
        {
            printf("Read from client error:%s\n",strerror(errno));
            close(info_set->accept_fd);
            return NULL;
        }	
        else if(retval == 0)
        {
            printf("Client lost connection\n");
            close(info_set->accept_fd);
            return NULL;
        }	

        printf("Recv Tag is 0x%02x 0x%02x\n",info_set->str_data[0],info_set->str_data[1]);	
        printf("Recv str_data[2]:0x%02x\n",info_set->str_data[2]);	

        //遍历
        printf("traverse:\n");
        int    j = 0;
        for(j=0;j<strlen(info_set->str_data);j++)
        {
            printf("0x%02x  ",info_set->str_data[j]);
        }
        printf("\n");

        //截取username和password
        memset(name,0,sizeof(name));
        memset(pawd,0,sizeof(pawd));
        if((p = strstr(info_set->str_data,":")) != NULL)
        {
            memcpy(name,info_set->str_data+ 2,p-(info_set->str_data+ 2));
            memcpy(pawd,p+1,info_set->str_data+strlen(info_set->str_data)-p);		
            printf("Recv username:%s\n",name);
            printf("Recv password:%s\n",pawd);
        }
        else
        {
            printf("Cannot find username && password\n");
            printf("Try again\n");
            continue;
        }
        //校验Tag:0x2f 0x3f
        if(info_set->str_data[0] == 0x2f && info_set->str_data[1] == 0x3f)
        {

            for(i=0; i<USER_NUM;i++)
            {
                if(!strcmp(name,username[i]) && !strcmp(pawd,password[i]))
                {
                    //登陆成功
                    printf("Login in successfully\n");
                    printf("username:%s\n",username[i]);
                    printf("password:%s\n",password[i]);
                    printf("Return sign_RIG:0x%02x 0x%02x 0x%02x 0x%02x\n",sign_RIG[0],sign_RIG[1],sign_RIG[2],sign_RIG[3]);					
                    send(info_set->accept_fd,sign_RIG,sizeof(sign_RIG),0);

                    while(1)					//收发数据
                    {
                        retval = 0;
                        memset(info_set->str_data,0,sizeof(info_set->str_data));
                        if((retval = recv(info_set->accept_fd,info_set->str_data,sizeof(info_set->str_data),0)) == -1)
                        {
                            printf("Recv failure: %s\n",strerror(errno));
                            sleep(1);
                            continue;
                        }	
                        else if(retval > 0)
                        {
                            //校验Tag:0x2f 0x3f
                            if(info_set->str_data[0] == 0x2f && info_set->str_data[1] == 0x3f)
                            {
                                switch(info_set->str_data[2])
                                {
                                    case  0x4f:			//请求打开
                                        if((fp = popen("sudo ./video_start &","r")) == NULL)
                                            printf("Popen ON failure: %s\n",strerror(errno));
                                        else
                                        {
                                            printf("Popen ON success\n");
                                            send(info_set->accept_fd,sign_ON ,sizeof(sign_ON ),0);
                                            printf("Return sign_RIG:0x%02x 0x%02x 0x%02x 0x%02x\n",sign_RIG[0],sign_RIG[1],sign_RIG[2],sign_RIG[3]);
                                        }	
                                        pclose(fp);	
                                        break;
                                    case  0x5f:			//请求关闭
                                        if((fp = popen("sudo killall -2 mjpg_streamer","r")) == NULL)
                                            printf("Popen OFF failure: %s\n",strerror(errno));
                                        else
                                        {
                                            printf("Popen OFF success\n");
                                            send(info_set->accept_fd,sign_OFF,sizeof(sign_OFF),0);
                                            printf("Return sign_RIG:0x%02x 0x%02x 0x%02x 0x%02x\n",sign_RIG[0],sign_RIG[1],sign_RIG[2],sign_RIG[3]);
                                        }	
                                        pclose(fp);	
                                        break;
                                    case  0x6f:			//请求查询	
                                        if((fp = popen("ps aux |grep -v \"grep\"| grep mjpg_streamer | awk '{print $11}'","r")) == NULL)		//获取进程名
                                            printf("Popen ASK failure: %s\n",strerror(errno));
                                        else
                                        {
                                            char   pid[32];
                                            memset(pid,0,sizeof(pid));
                                            if(fgets(pid,sizeof(pid),fp) < 0)
                                            {
                                                printf("Fgets from popen() failure: %s\n",strerror(errno));
                                            }	
                                            else
                                            {
                                                if(strstr(pid,"mjpg_streamer") != NULL)
                                                {
                                                    send(info_set->accept_fd,sign_ON ,sizeof(sign_ON ),0);
                                                    printf("Return sign_ON:0x%02x 0x%02x 0x%02x 0x%02x\n",sign_ON[0],sign_ON[1],sign_ON[2],sign_ON[3]);
                                                }
                                                else
                                                {															
                                                    send(info_set->accept_fd,sign_OFF,sizeof(sign_OFF),0);
                                                    printf("Return sign_OFF:0x%02x 0x%02x 0x%02x 0x%02x\n",sign_OFF[0],sign_OFF[1],sign_OFF[2],sign_OFF[3]);
                                                }	
                                                printf("Popen ASK success\n");	
                                            }	

                                        }	
                                        pclose(fp);	
                                        break;		
                                    default:										
                                        printf("Receive uncertain information:0x%02x 0x%02x\n",info_set->str_data[2],info_set->str_data[3]);	
                                        break;												
                                }
                            }	
                        }
                        else if(retval == 0)
                        {
                            printf("Client lost connection\n");
                            close(info_set->accept_fd);
                            return NULL;
                        }		
                    }
                }	
            }
            printf("Username or password is not right\n");
            send(info_set->accept_fd,sign_ERR,sizeof(sign_ERR),0);
        }
        else
        {
            printf("Check Tag failure\n");
            printf("Recv Tag is 0x%02x 0x%02x\n",info_set->str_data[0],info_set->str_data[1]);
            send(info_set->accept_fd,sign_ERR,sizeof(sign_ERR),0);
            printf("Return sign_ERR:0x%02x 0x%02x 0x%02x 0x%02x\n",sign_ERR[0],sign_ERR[1],sign_ERR[2],sign_ERR[3]);
        }	

    }

    close(info_set->accept_fd);
    return NULL;
}






