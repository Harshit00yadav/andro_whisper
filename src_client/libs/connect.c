#include "connect.h"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int initialize_connection(const char *ip_str, int port){
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	if (inet_pton(AF_INET, ip_str, &serv_addr.sin_addr) <=0) {
		fprintf(stderr, "[ ERROR ] Invalid ip address\n");
		return -1;
	}

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
		fprintf(stderr, "[ ERROR ] Connection failed\n");
		return -1;
	}
	return sockfd;
}

void send_chars(int sockfd, const char *word){
	send(sockfd, word, strlen(word), 0);
}
