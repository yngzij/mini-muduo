//
// Created by YngziJ on 12/18/2023.
//

#include "Acceptor.h"

#include <errno.h>

Acceptor::Acceptor(EventLoop *loop, const struct sockaddr_in &listenAddr) {
    loop_ = loop;
    acceptChannel_.setFd(socket_bind_listen(listenAddr));
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
    acceptChannel_.enableReading();
}

void Acceptor::handleRead() {
    loop_->assertInLoopThread();
    struct sockaddr_in peerAddr;
    //FIXME loop until no more
    int connfd = socket_accept(acceptChannel_.fd(), &peerAddr);
    if (connfd >= 0) {
        // string hostport = peerAddr.toIpPort();
        // LOG_TRACE << "Accept
        // s of " << hostport;
    }
}