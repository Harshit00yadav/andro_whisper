#include "connect.h"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

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
	char request[BUFFER_SIZE];
	size_t word_len = strlen(word);
	
	snprintf(request, sizeof(request),
		"POST /type HTTP/1.1\r\n"
		"Host: localhost:%d\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: %zu\r\n"
		"Connection: close\r\n"
		"\r\n"
		"%s",
		1337, word_len, word);
	
	send(sockfd, request, strlen(request), 0);
	
	char response[BUFFER_SIZE];
	int bytes_read = read(sockfd, response, sizeof(response) - 1);
	if (bytes_read > 0) {
		response[bytes_read] = '\0';
		if (strncmp(response, "HTTP/1.1 200", 12) != 0) {
			fprintf(stderr, "[ ERROR ] Server returned error\n");
		}
	}
}
