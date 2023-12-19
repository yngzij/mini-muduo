//
// Created by YngziJ on 2023/12/19.
//

#ifndef UNTITLED8_SOCKETS_H
#define UNTITLED8_SOCKETS_H


#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <cassert>


namespace sockets {
    int createNonblockingOrDie(sa_family_t family);
};


#endif //UNTITLED8_SOCKETS_H
