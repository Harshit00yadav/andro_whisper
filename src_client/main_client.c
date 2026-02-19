#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "libs/connect.h"
#include "libs/interminal.h"

void print_help();

int main(int argc, char **argv){
	if (argc < 2){
		print_help();
		return -1;
	}
	char *ip_addr_str = argv[1];
	int connfd;
	if ((connfd = initialize_connection(ip_addr_str, 1337)) < 0){
		fprintf(stderr, "[ ERROR ] Connection initialization failed\n");
		return -1;
	}
	fprintf(stdout, "[ INFO ] Connetion initialization successful\n");
	struct termios orig;
	set_raw_mode(&orig);

	char word_buff[1024] = {0};
	char c;
	int i = 0;
	bool EXIT = false;
	bool timeout_hit = false;
	while (!EXIT){
		int valid_char = read_char_timeout(3);
		if (valid_char < 0) fprintf(stderr, "[ ERROR ] read_char_timeout failed\n");
		if (valid_char == 0) {
			if (timeout_hit) continue;
			c = ' ';
			timeout_hit = true;
		} else {
			c = (unsigned char)valid_char;
			timeout_hit = false;
		}

		if (i >= sizeof(word_buff) - 1){
			printf("i: %d [ BUFFER OVERFLOW ]\n", i);
			c = ' ';
		}
		switch(c){
			case '\n':
			case ' ':
				word_buff[i] = '\0';
				send_chars(connfd, word_buff);
				memset(word_buff, 0, sizeof(word_buff));
				i = 0;
				break;
			case '~':
				EXIT = true;
				close(connfd);
				break;
			default:
				word_buff[i++] = c;
		}
	}
	restore_mode(&orig);
	return 0;
}

void print_help(){
	printf("Usage:\n");
	printf("tool <ip_addr>\n");
}
