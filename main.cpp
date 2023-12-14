#include "net/EventLoop.h"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>


int main() {
    EventLoop loop;

    int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080);

    if (bind(listen_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        perror("bind");
        close(listen_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(listen_socket, 128) == -1) {
        perror("listen");
        close(listen_socket);
        exit(EXIT_FAILURE);
    }

    //loop.loop();
    return 0;
}
