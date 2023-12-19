//
// Created by YngziJ on 2023/12/18.
//

#ifndef UNTITLED8_TCPSERVER_H
#define UNTITLED8_TCPSERVER_H


#include <string>
#include <map>

#include "Callback.h"
#include "Atomic.h"
#include "Callbacks.h"


class EventLoop;


class Acceptor;


class TcpServer {
public:
    TcpServer(EventLoop *loop, const struct sockaddr_in &listenAddr);
    ~TcpServer();

    void start();

    void setConnectionCallback(const ConnectionCallback &cb) { connectionCallback_ = cb; }

    /// Set message callback.
    /// Not thread safe.
    void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; }

    /// Set write complete callback.
    /// Not thread safe.
    void setWriteCompleteCallback(const WriteCompleteCallback &cb) { writeCompleteCallback_ = cb; }

private:
    /// Not thread safe, but in loop
    void newConnection(int sockfd, const struct sockaddr_in &peerAddr);
    /// Thread safe.
    void removeConnection(const TcpConnectionPtr &conn);
    /// Not thread safe, but in loop
    void removeConnectionInLoop(const TcpConnectionPtr &conn);

    typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

    const std::string ipPort_;
    const std::string name_;
    EventLoop *loop_;
    int nextConnId_;
    ConnectionMap connections_;
    std::unique_ptr<Acceptor> acceptor_; // avoid revealing Acceptor
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    //ThreadInitCallback threadInitCallback_;
    AtomicInt32 started_;
};


#endif //UNTITLED8_TCPSERVER_H
