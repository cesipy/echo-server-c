#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define BUF_SIZE 1024

void cleanup(int sockfd, int new_socket);
int setup_socket(int port, struct sockaddr_in *server);
void signal_handler(int signr);


int sockfd, new_socket;

int main(int argc, char* argv[])
{
    if (argc != 2) 
    { 
        fprintf(stderr ,"Incorrect usage! Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port;
    char* endptr;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char buffer[BUF_SIZE];

    port = strtol(argv[1],&endptr, 10);
    if (*endptr != '\0')
    { 
        fprintf(stderr, "failed to convert number %s.\n", argv[1] );
        exit(EXIT_FAILURE);
    }

    sockfd = setup_socket(port, &server);
    printf("Server connected to port %d\n", port);

    /**
     * set up a singal handler to handle CTRL + C
     * if SIGINT is received, the server is shut down gracefully
    */
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }
    
    int stop_flag = 0;
    /**
      * while loop waits for incoming connections
      * as this is only single threaded, only one client at a time
      * can connect to the server.
      */
    while(!stop_flag) 
    {
        socklen_t addrlen = sizeof(client);

        // accept new incoming connections from sockfd
        new_socket = accept(sockfd, (struct sockaddr*)&client, &addrlen);
        if (new_socket <0) 
        {
            perror("accept client");
            cleanup(sockfd, new_socket);
        }
        printf("Connected!\n");

        // loop for the connection
        for (;;) 
        {
            memset(buffer, 0, BUF_SIZE);        // reset buffer
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


int setup_socket(int port, struct sockaddr_in *server) 
{
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
        cleanup(sockfd, -1);
    }

    memset(server, 0, sizeof(struct sockaddr_in));
    server->sin_family = AF_INET;
    server->sin_port = htons(port);
    server->sin_addr.s_addr = htonl(INADDR_ANY);

    int bind_status = bind(sockfd, (struct sockaddr*)server, 
        sizeof(*server));
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
    return sockfd;
}


void cleanup(int sockfd, int new_socket) 
{
    if (new_socket != -1) 
        close(new_socket);
    if (sockfd != -1) 
        close(sockfd);
    exit(EXIT_FAILURE);
}


void signal_handler(int signr) 
{
    if (signr == SIGINT) 
    {
        char* message = "received CTRL + C. Shutting down the server!\n";
        size_t message_len = strlen(message);
        write(STDOUT_FILENO, message, message_len);
        cleanup(sockfd, new_socket);
    }
}
