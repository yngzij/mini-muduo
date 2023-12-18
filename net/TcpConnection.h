//
// Created by YngziJ on 2023/12/18.
//

#ifndef UNTITLED8_TCPCONNECTION_H
#define UNTITLED8_TCPCONNECTION_H


#include <string>

#include <memory>
#include "Buffer.h"
#include "Callbacks.h"


class EventLoop;


class Channel;


class Timestamp;


class TcpConnection : noncopyable,
                      public std::enable_shared_from_this<TcpConnection> {

public:
    TcpConnection(EventLoop *loop, std::string name, int sockfd, const sockaddr_in localAddr,
                  const sockaddr_in peerAddr);

    void shutdownInLoop();

    ~TcpConnection();
private:
    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();

    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    HighWaterMarkCallback highWaterMarkCallback_;
    CloseCallback closeCallback_;

    enum StateE {
        kDisconnected, kConnecting, kConnected, kDisconnecting
    };
    EventLoop *loop_;
    const std::string name_;
    StateE state_;
    int sockfd_;
    std::unique_ptr<Channel> channel_;
    const sockaddr_in localAddr_;
    const sockaddr_in peerAddr_;

    Buffer inputBuffer_;
    Buffer outputBuffer_; // FIXME: use list<Buffer> as output buffer.

};


#endif //UNTITLED8_TCPCONNECTION_H
