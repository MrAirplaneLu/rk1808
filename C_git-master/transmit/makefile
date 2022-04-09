#9个文件，分别为client、server_SSL、transmit的函数，完成普通socket<--->SSL_socket的双向传输，实现加密传输的功能

#client的传参有:
#               client
#               client [addrip] [port]

#server的传参有:
#               server   
#               server [port]

#transmit的传参有:
#               transmit 
#               transmit C [addrip] [port]
#               transmit S [port] 
#               transmit C [addrip] [port] S [port] 

#参数[port]已校正为只能是数字




all:
	gcc client.c     client.h     client_main.c     -lssl -lpthread -Wall -Werror -o client
	gcc server_SSL.c server_SSL.h server_SSL_main.c -lssl -lpthread -Wall -Werror -o server_SSL
	gcc transmit.c   transmit.h   transmit_main.c   -lssl -lpthread -Wall -Werror -o transmit
	make show

show:
	@echo "                                                      "
	@echo "                   文  件  说  明                     "
	@echo "9个文件，分别为client、server_SSL、transmit的函数，完成普通socket<--->SSL_socket的双向传输，实现加密传输的功能"
	@echo "client的传参有:                                       "
	@echo "               client                                 "
	@echo "               client [addrip] [port]                 "
	@echo "                                                      "
	@echo "server的传参有:                                       "
	@echo "               server                                 "
	@echo "               server [port]                          "
	@echo "                                                      "
	@echo "transmit的传参有:                                     "
	@echo "               transmit                               "
	@echo "               transmit C [addrip] [port]             "
	@echo "               transmit S [port]                      "
	@echo "               transmit C [addrip] [port] S [port]    "
	@echo "                                                      "
	@echo "参数[port]已校正为只能是数字                          "
	@echo "执行过程中，输入 quit 退出                                        "
