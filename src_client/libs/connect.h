#ifndef CONNECT_H
#define CONNECT_H

int initialize_connection(const char *ip_str, int port);
void send_chars(int sockfd, const char *word);

#endif
