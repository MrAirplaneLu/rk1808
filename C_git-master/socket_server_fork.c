/*********************************************************************************
 *      Copyright:  (C) 2018 Yujie
 *                  All rights reserved.
 *
 *       Filename:  socket_server.c
 *    Description:  server.c
 *                 
 *        Version:  1.0.0(04/21/2018)
 *         Author:  yanhuan <yanhuanmini@foxmail.com>
 *      ChangeLog:  1, Release initial version on "04/21/2018 09:21:24 PM"
 *                 
 ********************************************************************************/
/* 
  1、创建套接字---socket
     处理地址和端口
  2、套接字绑定本地的地址和端口---band
  3、套接字设定监听状态（等待客服端消息）---listen
  4、接受消息，返回一个用连接的新套接字
  5、新套接字通信---read，write
  6、关闭套接字---close
*/

#include <sys/socket.h>  //socket头文件
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define port     12000
#define buf_size 1024


int main (int argc, char **argv)
{
    int                      listen_fd, accept_fd = 1;  //服务器端需要两个套接字：一个开启连接，一个通信
    struct sockaddr_in       server_addr;            //服务器端地址，是一个结构体，等下要填充ipV4或者ipV6的结构
    char                     buf[buf_size];
    int                      pid;

    listen_fd = socket(AF_INET,SOCK_STREAM,0);       //创建套接字，ipV4地址，可靠的面向连接的字符流，最后0是默认协议
    if(listen_fd < 0)
    {
        printf("Create socket failure: %s\n", strerror(errno));  //如果创建失败，打印错误信息
        return -1;
    }
    printf("Socket create fd[%d]\n",listen_fd);      //如果成功，返回fd
   
	//服务器设置，设置server地址和端口号，用于接受任何client端
    memset(&server_addr,0,sizeof(server_addr));       //清空再设置ipV4地址
    server_addr.sin_family = AF_INET;                //设置为ipV4地址
    server_addr.sin_port = htons(port);             //端口号，主机字节序转网络字节序,short是16位
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //设置host主机地址，INADDR_ANY是指定为0.0.0.0的地址，表示不确定地址，或指所有地址
//	server_addr.sin_addr.s_addr = htonl(ip);    
    if( bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 )
    {
        printf("Bind socket failure: %s\n", strerror(errno));  //如果绑定失败，打印错误信息
        return -2;
    }
    printf("Socket bind ok \n");
    
    listen(listen_fd,15);               //等待连接，最大连接数设为15 
    printf("Socket listen ok \n");
    
    while(1)
    {
        printf("Start accept %d...\n",listen_fd);
        accept_fd = accept(listen_fd,NULL,NULL);
        //accept接受的参数必须经过listen和bing处理过，返回一个新的socket码
        //新的socket传送数据，旧的socket还能继续用accept接受请求
        //后面两个参数设置为NULL，那么会在系统连接时自动根据客户端信息填入
        if(accept_fd < 0)
        {
            printf("Accept socket failure: %s\n", strerror(errno));  //如果监听失败，打印错误信息
            return -1;  
        }
        printf("Socket Accept fd[%d]\n",accept_fd);      //如果成功，返回fd
        
        //多进程，高并发
        pid = fork();
        if(pid < 0)
        {
            printf("Fork failure: %s\n", strerror(errno));
            return -1;
        }

        if(pid == 0)
        {
            memset(buf, 0, sizeof(buf)); 
        
            read(accept_fd,buf,sizeof(buf));				//从套接字read
            printf("From client read : %s\n",buf);
		
            write(accept_fd,"I have received the message.",strlen("I have received the message."));	//写回给套接字
            sleep(1);

            close(accept_fd);
        }
        else
            close(listen_fd);
    }
    return 0;
} /* ----- End of main() ----- */



