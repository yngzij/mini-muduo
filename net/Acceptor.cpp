//
// Created by YngziJ on 12/18/2023.
//

#include "Acceptor.h"

#include <errno.h>
#include <sys/fcntl.h>
#include "EventLoop.h"

#include "Sockets.h"


Acceptor::Acceptor(EventLoop *loop, const struct sockaddr_in &listenAddr) :
        loop_(loop),
        acceptChannel_(loop, acceptSocket_),
        acceptSocket_(sockets::createNonblockingOrDie(listenAddr.sin_family)),
        listening_(false),
        idleFd_(open("/dev/null", O_RDONLY | O_CLOEXEC)
        ) {
    loop_ = loop;
    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));

}

Acceptor::~Acceptor() {
    acceptChannel_.disableAll();
    acceptChannel_.remove();
    ::close(idleFd_);
}

void Acceptor::listen() {
    loop_->assertInLoopThread();
    listening_ = true;
    ::listen(acceptSocket_, 20);
    acceptChannel_.enableReading();
}

void Acceptor::handleRead() {
    loop_->assertInLoopThread();
    struct sockaddr_in peerAddr;
    //FIXME loop until no more
    socklen_t peerAddrLen = sizeof(peerAddr);
    int connfd = accept(acceptSocket_, (struct sockaddr *) &peerAddr, &peerAddrLen);

    if (connfd >= 0) {
        if (newConnectionCallback_) {
            newConnectionCallback_(connfd, peerAddr);
        } else {
            close(connfd);
        }
    }
}