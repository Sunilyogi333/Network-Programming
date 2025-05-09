#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#define MAXLINE 1024
#define LISTENQ 10
typedef struct sockaddr SA;
int main(int argc, char **argv)
{
    int sockfd, n;
    char recvline[MAXLINE + 1];
    char buff[MAXLINE];
    struct sockaddr_in servaddr;
    int port;

    if (argc != 3)
        printf("usage: a.out <IPaddress> <port no.>");

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        printf("socket error");
    port = atoi(argv[2]);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port); /* daytime server */
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        printf("inet_pton error for %s", argv[1]);
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
        printf("connect error");
    while (1)
    {
        printf("Enter text");
        fgets(buff, MAXLINE, stdin);
        write(sockfd, buff, MAXLINE);
        printf("Eco from server: \t");
        read(sockfd, recvline, MAXLINE);
        
        puts(recvline);
    }
}
