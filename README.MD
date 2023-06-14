# Simple Echo Server

This C program serves as a simple, single-threaded, TCP echo server. The program accepts client connections, receives messages from the client, and echoes those messages back to the client. When a client sends a `/close` command, the server will shutdown.

## How to compile

You can compile the program using the following command:

```bash
gcc server.c -o server
```

This will create an executable named `server` in your current directory.

## How to run

Once you've compiled the server, you can run it with a single argument, the port number on which the server should listen. For example, to run the server on port 8080, use the following command:

```bash
./server 8080
```

The server will then listen for incoming connections on port 8080. It can handle one client at a time. Remember: Port 0-1023 are priviliged and should not be used!

You can use netcat to connect to the server using the following command:

```bash
nc localhost <port>
````

## Behavior

The server waits for incoming connections, and once a client is connected, it enters a loop where it reads data from the client and echoes it back. The server keeps track of a "stop flag" which, when set, causes the server to shutdown. The stop flag is set when a client sends a `/close` command.

## Error Handling

In case of any error during the creation, binding, or listening on the socket, the program will print an error message and terminate. The `cleanup` function is used to close any open sockets before exiting.

## Limitations

This server is single-threaded, so it can only handle one client at a time. If a client is currently connected, other clients attempting to connect will have to wait until the current client disconnects. 
