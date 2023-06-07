#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024

void cleanup(int sockfd, int new_socket);

int main(int argc, char* argv[]){
    if (argc != 2) { exit(EXIT_FAILURE);}

    int port;
    char* endptr;

    port = strtol(argv[1],&endptr, 10);
    if (*endptr != '\0'){ exit(EXIT_FAILURE);}

    int sockfd;

    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t addrlen = sizeof(client);
    char buffer[BUF_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
        cleanup(sockfd, -1);
    }

    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    int bind_status = bind(sockfd, (struct sockaddr*)&server, 
        sizeof(server));
    if (bind_status == -1) 
    {
        perror("bind error");
        cleanup(sockfd, -1);
    }

    if (listen(sockfd, 10) != 0) 
    {
        perror("listen error");
        cleanup(sockfd, -1);
    }
    printf("Server connected to port %d\n", port);

    
    int stop_flag = 0;
    /*
     * while loop waits for incoming connections
     * as this is only single threaded, only one client at a time
     * can connect to the server.
     * 
     * if stop_flag is set -> shutdown of the server
     * stop_flag is set when a client types '/close'
     */
    while(!stop_flag) 
    {
        int new_socket = accept(sockfd, (struct sockaddr*)&client, &addrlen);
        if (new_socket <0) 
        {
            perror("accept client");
            cleanup(sockfd, new_socket);
        }
        printf("Connected!\n");

        // loop for clonnection
        for (;;) 
        {
            memset(buffer, 0, BUF_SIZE);
            int status_read = read(new_socket, buffer, BUF_SIZE - 1);
            if (status_read <0)
            {
                perror("error reading buffer");
                cleanup(sockfd, new_socket);
            }
            else if (status_read == 0) 
            { break; }

            char* shutdown_message = "/close";
            if (!strncmp(buffer, shutdown_message, strlen(shutdown_message))) 
            {
                printf("Received shutdown command!\nClosing connection...");
                stop_flag = 1;
                break;
            }

            // write echo message to client
            else 
            {
                dprintf(new_socket, "echo: %s\n", buffer);
            }
        }
        close(new_socket);
    }
    close(sockfd);
    return 0;
}


void cleanup(int sockfd, int new_socket) 
{
    if (new_socket != -1) close(new_socket);
    if (sockfd != -1) close(sockfd);
    exit(EXIT_FAILURE);
}
