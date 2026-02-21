#include "host.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 4096

static const char *HTTP_OK_RESPONSE = 
	"HTTP/1.1 200 OK\r\n"
	"Content-Type: text/plain\r\n"
	"Content-Length: 2\r\n"
	"Connection: close\r\n"
	"\r\n"
	"OK";

static const char *HTTP_BAD_REQUEST_RESPONSE = 
	"HTTP/1.1 400 Bad Request\r\n"
	"Content-Type: text/plain\r\n"
	"Content-Length: 2\r\n"
	"Connection: close\r\n"
	"\r\n"
	"ER";

static int parse_http_request(const char *buffer, size_t buffer_len, char *body, size_t body_max_len) {
	const char *crlf = strstr(buffer, "\r\n\r\n");
	if (!crlf) {
		return -1;
	}
	
	const char *body_start = crlf + 4;
	size_t body_len = buffer_len - (body_start - buffer);
	
	if (body_len >= body_max_len) {
		body_len = body_max_len - 1;
	}
	
	memcpy(body, body_start, body_len);
	body[body_len] = '\0';
	
	if (strncmp(buffer, "POST ", 5) != 0) {
		return -1;
	}
	
	return (int)body_len;
}

int initialize_hosting(int port){
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		fprintf(stderr, "[ ERROR ] Socket creation failed\n");
		return -1;
	}
	
	int opt = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		fprintf(stderr, "[ ERROR ] Bind failed\n");
		close(sockfd);
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
	printf("Listening on port %d (HTTP)...\n", 1337);
	while (1){
		clientfd = accept(sockfd, (struct sockaddr *)&address, &addr_len);
		if (clientfd < 0){
			fprintf(stderr, "[ ERROR ] client accept failed\n");
			continue;
		}
		printf("Connection from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
		
		int bytes_read = read(clientfd, buffer, buffer_size - 1);
		if (bytes_read > 0) {
			buffer[bytes_read] = '\0';
			
			char body[buffer_size];
			int body_len = parse_http_request(buffer, bytes_read, body, sizeof(body));
			
			if (body_len > 0) {
				printf("Received POST body: %s\n", body);
				FILE *fp = popen("xdotool type --file -", "w");
				if (fp) {
					fprintf(fp, "%s", body);
					pclose(fp);
				}
				write(clientfd, HTTP_OK_RESPONSE, strlen(HTTP_OK_RESPONSE));
			} else {
				fprintf(stderr, "[ ERROR ] Invalid HTTP request\n");
				write(clientfd, HTTP_BAD_REQUEST_RESPONSE, strlen(HTTP_BAD_REQUEST_RESPONSE));
			}
		}
		close(clientfd);
	}
	close(sockfd);
}
