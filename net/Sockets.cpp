//
// Created by YngziJ on 2023/12/19.
//

#include "Sockets.h"

namespace sockets {
    int createNonblockingOrDie(sa_family_t family) {
        int sockfd = socket(family, SOCK_STREAM, IPPROTO_TCP);

        if (sockfd >= 0) {
            int flags = fcntl(sockfd, F_GETFD);
            if (flags != -1) {
                flags |= FD_CLOEXEC;
                fcntl(sockfd, F_SETFD, flags);
            }
        }

        // set non-blocking
        if (sockfd >= 0) {
            int flags = fcntl(sockfd, F_GETFL);
            if (flags != -1) {
                flags |= O_NONBLOCK;
                fcntl(sockfd, F_SETFL, flags);
            }
        }

        // set reuse addr
        int reuse = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
        // set reuse port
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse));

        // bind ipv4 9090 port to sockfd
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(9090);
        addr.sin_addr.s_addr = INADDR_ANY;
        int err = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
        assert(err >= 0);

        return sockfd;
    }
}