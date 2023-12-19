//
// Created by YngziJ on 2023/12/18.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

#include "TcpServer.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "TcpConnection.h"


TcpServer::TcpServer(EventLoop *loop, const struct sockaddr_in &addr)
        : loop_(loop),
          acceptor_(
                  new Acceptor(loop, addr)) {
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1,
                                                  std::placeholders::_2));


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

    TcpConnectionPtr newConnection(new TcpConnection(
            loop_,
            connName,
            sockfd,
            localaddr,
            peerAddr
    ));

    connections_[connName] = newConnection;
    newConnection->setConnectionCallback(connectionCallback_);
    newConnection->setMessageCallback(messageCallback_);
    newConnection->setWriteCompleteCallback(writeCompleteCallback_);

    newConnection->setCloseCallback(
            std::bind(&TcpServer::removeConnection, this, std::placeholders::_1)); // FIXME: unsafe
    loop_->runInLoop(std::bind(&TcpConnection::connectEstablished, newConnection));

}

void TcpServer::removeConnection(const TcpConnectionPtr &conn) {
    printf("TcpServer::removeConnection [%s] - connection %s\n", name_.c_str(), conn->name().c_str());

    loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr &conn) {
    loop_->assertInLoopThread();
    /*LOG_INFO << "TcpServer::removeConnectionInLoop [" << name_
             << "] - connection " << conn->name();*/
    size_t n = connections_.erase(conn->name());
    (void) n;
    assert(n == 1);
    EventLoop *ioLoop = conn->getLoop();
    ioLoop->queueInLoop(
            std::bind(&TcpConnection::connectDestroyed, conn));
}

void TcpServer::start() {
    if (started_.getAndSet(1) == 0) {
        // 执行acceptor的listen
        //threadPool_->start(threadInitCallback_);

        assert(!acceptor_->listening());
        // 执行acceptor的listen
        loop_->runInLoop(
                std::bind(&Acceptor::listen, get_pointer(acceptor_)));
    }
}

