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
#include <openssl/ssl.h>
#include <openssl/err.h>
#define port     12000
#define buf_size 1024


int main (int argc, char **argv)
{
    int                      listen_fd, accept_fd = 1;  //服务器端需要两个套接字：一个开启连接，一个通信
    struct sockaddr_in       server_addr;            //服务器端地址，是一个结构体，等下要填充ipV4或者ipV6的结构
    char                     buf[buf_size];
    
    SSL_CTX *ctx;
    
         /*  SSL 库初始化 */
    SSL_library_init();
    /*  载入所有 SSL 算法 */
    OpenSSL_add_all_algorithms();
    /*  载入所有 SSL 错误消息 */
    SSL_load_error_strings();
    /*  以 SSL V2 和 V3 标准兼容方式产生一个 SSL_CTX ，即 SSL Content Text */
    ctx = SSL_CTX_new(SSLv23_server_method());
    /*  也可以用 SSLv2_server_method() 或 SSLv3_server_method() 单独表示 V2 或 V3标准 */
    if (ctx == NULL) 
   	{
        printf("OpenSSL load failure: %s\n", strerror(errno));  //如果创建失败，打印错误信息
        return -1;
    }
    
    /*  载入用户的数字证书， 此证书用来发送给客户端。 证书里包含有公钥 */
    if (SSL_CTX_use_certificate_file(ctx, "./cacert.pem", SSL_FILETYPE_PEM) <= 0)
    {
    		printf("Load use_cert failure: %s\n", strerror(errno));
    		return -1;
    }
    /*  载入用户私钥 */
    if (SSL_CTX_use_PrivateKey_file(ctx, "./privkey.pem", SSL_FILETYPE_PEM) <= 0)
    {
    		printf("Load use_Priv failure: %s\n", strerror(errno));
    		return -1;
    }
    /*  检查用户私钥是否正确 */
    if (!SSL_CTX_check_private_key(ctx))
    {
    		printf("Check use_Priv error: %s\n", strerror(errno));
    		return -1;
   	}       
    
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
        
        SSL *ssl;
        
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
        
        /*  基于 ctx 产生一个新的 SSL */
        ssl = SSL_new(ctx);
        /*  将连接用户的 socket 加入到 SSL */
        SSL_set_fd(ssl,accept_fd);
        /*  建立 SSL 连接 */
        if( SSL_accept(ssl) == -1 )
        {
        	  printf("Accept SSL-socket failure: %s\n", strerror(errno)); 
        	  close(accept_fd);
        	  break;  
        }      
     
        memset(buf, 0, sizeof(buf)); 
        
        if( SSL_read(ssl,buf,sizeof(buf)) <=0 )			//从套接字read
        {
        		printf("消息'%s'接收失败！错误代码是%d，错误信息是'%s'\n", buf, errno, strerror(errno));
        		goto finish;
        }		
        else
        		printf("From client read : %s\n",buf);
		
        SSL_write(ssl,"I have received the message.",strlen("I have received the message."));	//写回给套接字
        sleep(1);
    
finish:
		/*  关闭 SSL 连接 */
		SSL_shutdown(ssl);
		/*  释放 SSL */
		SSL_free(ssl);
		}
    close(listen_fd);

		/*  释放 CTX */
		SSL_CTX_free(ctx);
    return 0;
} /* ----- End of main() ----- */



