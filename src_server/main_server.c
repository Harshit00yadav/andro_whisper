#include <stdio.h>
#include "libs/host.h"

int main(){
	int connfd;
	if ((connfd = initialize_hosting(1337)) < 0){
		fprintf(stderr, "[ ERROR ] Connection initialization failed\n");
		return -1;
	}
	fprintf(stdout, "[ INFO ] Connetion initialization successful\n");
	start_listening(connfd);
	return 0;
}
