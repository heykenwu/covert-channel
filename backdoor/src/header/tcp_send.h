#ifndef TCP_SEND_H
#define TCP_SEND_H

#include <stdio.h>

void send_file(FILE *fp, int sockfd);
void exfilt_file(char * lhost, int lport, char* _filename);

#endif