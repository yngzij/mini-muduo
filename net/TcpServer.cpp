//
// Created by YngziJ on 2023/12/18.
//

#include <sys/socket.h>
#include <netinet6/in6.h>
#include <netinet/in.h>
#include "TcpServer.h"

#include <string>
#include "EventLoop.h"

#include "TcpConnection.h"


TcpServer::TcpServer(EventLoop *loop) : loop_(loop) {

}

TcpServer::~TcpServer() {

}

void TcpServer::newConnection(int sockfd, const struct sockaddr_in &peerAddr) {
    loop_->assertInLoopThread();
    //EventLoop *ioLoop = threadPool_->getNextLoop();

    char buf[64];

    std::string connName = name_ + buf;
    //LOG_INFO << "TcpServer::newConnection [" << name_
    struct sockaddr_in localaddr;
    memZero(&localaddr, sizeof localaddr);
    socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
    /*if (::getsockname(sockfd, sockaddr_cast(&localaddr), &addrlen) < 0) {
        LOG_SYSERR << "sockets::getLocalAddr";
    }*/

    TcpConnectionPtr tcpConnection(new TcpConnection(
            loop_,
            connName,
            sockfd,
            localaddr,
            peerAddr
    ));

    connections_[connName] = tcpConnection;


}

void TcpServer::removeConnection(const TcpConnectionPtr &conn) {

}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr &conn) {

}
