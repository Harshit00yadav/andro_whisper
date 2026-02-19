#include "host.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int initialize_hosting(int port){
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		fprintf(stderr, "[ ERROR ] Bind failed\n");
		return -1;
	}
	return sockfd;
}

void start_listening(int sockfd){
	struct sockaddr_in address;
	socklen_t addr_len = sizeof(address);
	int clientfd;
	size_t buffer_size = BUFFER_SIZE;
	char buffer[buffer_size];

	if (listen(sockfd, 1) < 0){
		fprintf(stderr, "[ ERROR ] listening failed\n");
		return;
	}
	printf("Listening...\n");
	while (1){
		clientfd = accept(sockfd, (struct sockaddr *)&address, &addr_len);
		if (clientfd < 0){
			fprintf(stderr, "[ ERROR ] client accept failed\n");
			continue;
		}
		printf("Connection from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
		int bytes_read;
		char command_buffer[buffer_size];
		while ((bytes_read = read(clientfd, buffer, buffer_size - 1)) > 0){
			buffer[bytes_read] = '\0';
			snprintf(command_buffer, sizeof(command_buffer), "xdotool type \"%s\"", buffer);
			system(command_buffer);
		}
	}
	close(sockfd);
}
