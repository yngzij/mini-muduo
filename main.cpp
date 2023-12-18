#include "net/EventLoop.h"
#include "Channel.h"

#include <iostream>
#include <sys/socket.h>
#include <sys/event.h>
#include <functional>
#include <netinet/in.h>


#define MAX_EVENTS 10

void readCallback(Timestamp receiveTime, int fd, int idx) {
    // accept new connection
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(fd, (struct sockaddr *) &client_addr, &client_addr_len);
    if (client_fd < 0) {
        std::cout << "accept error" << std::endl;
    } else {
        std::cout << "new connection" << std::endl;
        /*// add new connection to kqueue
        struct kevent changes[1];
        EV_SET(&changes[0], client_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
        int ret = kevent(fd, changes, 1, NULL, 0, NULL);
        if (ret < 0) {
            std::cout << "kevent error" << std::endl;
        }
         */
    }

    std::cout << "inReadCallback(): received " << receiveTime.toString() << std::endl;
}

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
    server_address.sin_port = htons(9090);

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

    Channel channel(&loop, listen_socket);
    //channel.setReadCallback(onReadCallback);

    channel.setReadCallback(std::bind(readCallback, std::placeholders::_1, channel.fd(), 2));
    channel.enableReading();

    loop.loop();

    return 0;
}
