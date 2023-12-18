//
// Created by YngziJ on 2023/12/18.
//

#include <netinet/in.h>

#include <utility>
#include <memory>
#include <errno.h>

#include "EventLoop.h"
#include "TcpConnection.h"
#include "Channel.h"


TcpConnection::TcpConnection(EventLoop *loop, std::string name, int sockfd, const struct sockaddr_in localAddr,
                             const struct sockaddr_in peerAddr) : loop_(loop), name_(std::move(name)),
                                                                  state_(kConnecting),
                                                                  sockfd_(sockfd),
                                                                  localAddr_(localAddr), peerAddr_(peerAddr),
                                                                  channel_(new Channel(loop, sockfd)) {
    channel_->setReadCallback(
            std::bind(&TcpConnection::handleRead, this, _1));
    channel_->setWriteCallback(
            std::bind(&TcpConnection::handleWrite, this));
    channel_->setCloseCallback(
            std::bind(&TcpConnection::handleClose, this));
    channel_->setErrorCallback(
            std::bind(&TcpConnection::handleError, this));
    //socket_->setKeepAlive(true);
}

void TcpConnection::handleRead(Timestamp receiveTime) {
    loop_->assertInLoopThread();
    int savedErrno = 0;
    ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
    if (n > 0) {
        messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
    } else if (n == 0) {
        handleClose();
    } else {
        errno = savedErrno;
        //LOG_SYSERR << "TcpConnection::handleRead";
        handleError();
    }
}

TcpConnection::~TcpConnection() {

}

void TcpConnection::handleWrite() {
    loop_->assertInLoopThread();
    if (channel_->isWriting()) {
        ssize_t n = sockets::write(channel_->fd(),
                                   outputBuffer_.peek(),
                                   outputBuffer_.readableBytes());
        if (n > 0) {
            outputBuffer_.retrieve(n);
            if (outputBuffer_.readableBytes() == 0) {
                channel_->disableWriting();
                if (writeCompleteCallback_) {
                    loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
                }
                if (state_ == kDisconnecting) {
                    shutdownInLoop();
                }
            }
        } else {
            //LOG_SYSERR << "TcpConnection::handleWrite";
            // if (state_ == kDisconnecting)
            // {
            //   shutdownInLoop();
            // }
        }
    } else {
        /*LOG_TRACE << "Connection fd = " << channel_->fd()
                  << " is down, no more writing";*/
    }
}

void TcpConnection::handleClose() {

}

void TcpConnection::shutdownInLoop() {
    loop_->assertInLoopThread();
    if (!channel_->isWriting()) {
        // we are not writing
        //socket_->shutdownWrite();
    }
}

void TcpConnection::handleError() {

}
