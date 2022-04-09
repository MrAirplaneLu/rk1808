all:
	gcc webcam_server.c -Wall -Werror -lpthread -o webcam_server
	gcc webcam_client.c -Wall -Werror -o  webcam_client
	chmod u+x video_start

clean:
	rm ./webcam_server ./webcam_client
