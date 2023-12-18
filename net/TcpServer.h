//
// Created by YngziJ on 2023/12/18.
//

#ifndef UNTITLED8_TCPSERVER_H
#define UNTITLED8_TCPSERVER_H


#include <string>
#include <map>

#include "Callback.h"
#include "Callbacks.h"


class EventLoop;


class TcpServer {
public:
    TcpServer(EventLoop *loop);
    ~TcpServer();

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
    ConnectionMap connections_;
};


#endif //UNTITLED8_TCPSERVER_H
