EXTRA_CFLAGS+= -Wall -Werror -lssl

all:
	gcc $(EXTRA_CFLAGS) socket_client_ssl.c -o client
	gcc $(EXTRA_CFLAGS) socket_server_ssl.c -o server

