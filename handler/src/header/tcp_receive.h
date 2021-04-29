#ifndef TCP_RECEIVE_H
#define TCP_RECEIVE_H

void write_file(int sockfd, char * filename);
int recv_file(int port, char * filename);

#endif