#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>

#define MAXLINE 1024
#define LISTENQ 10
typedef struct sockaddr SA;

int main(int argc, char *argv[])
{
    int listenfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    char buff[MAXLINE];
    time_t ticks;
    int port;
    socklen_t len;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    listenfd = socket(AF_INET, SOCK_STREAM, 0); // IPPROTO_TCP
    port = atoi(argv[1]);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port); /* daytime server */

    if (bind(listenfd, (SA *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }
    printf("Server is waiting for connections on port %d\n", port);

    if (listen(listenfd, LISTENQ) < 0) {
        perror("listen error");
        exit(EXIT_FAILURE);
    }

    len = sizeof(cliaddr);
    connfd = accept(listenfd, (SA *)&cliaddr, &len);
    printf("Connection from %s, port %d\n",
           inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, buff, sizeof(buff)),
           ntohs(cliaddr.sin_port));

    ticks = time(NULL);

    for (;;)
    {
        memset(buff, 0, MAXLINE);  // Clear the buffer

        // Read data from client
        read(connfd, buff, MAXLINE);

        printf("From client: \t");

        // Convert to uppercase
        for (int i = 0; buff[i] != '\0'; i++) {
            buff[i] = toupper(buff[i]);  // Convert each character to uppercase
        }

        // Print the converted message
        puts(buff);  // Display the modified string
        write(connfd, buff, strlen(buff));  // Send the converted string back to the client
    }
}
