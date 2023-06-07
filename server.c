#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char* argv[]){

    if (argc != 2) { exit(EXIT_FAILURE);}

    int port;
    char* endptr;

    port = strtol(argv[1],&endptr, 10);
    if (*endptr != '\0'){ exit(EXIT_FAILURE);}

    int sockfd;

    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t client;
    char buffer[BUF_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        exit(EXIT_FAILURE);
    }
    
    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);


    int bind_status = bind(sockfd, (struct sockaddr*)&server, sizeof(server));
    // TODO: ERROR handling


    if (listen(sockfd, 10) != 0) {
        perror("listen error");
        exit(EXIT_FAILURE);
    }
    printf("Server connected to port %d", port);

    int stop_flag = 0;

    while(!stop_flag) {
        int new_socket = accept(sockfd, (struct sockaddr*)&client, sizeof(client));
        if (new_socket <0) {
            perror("accept client");
            exit(EXIT_FAILURE);
        }
        printf("Connected!\n");

        //loop for each connection
        for (;;) {
            //reset buffer
            memset(buffer, 0, 1024);
            if 
        }
    }

}