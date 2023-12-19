//
// Created by YngziJ on 12/18/2023.
//

#ifndef MINI_MUDUO_FILESERVER_H
#define MINI_MUDUO_FILESERVER_H


#include "TcpServer.h"


class FileServer {
public:
    FileServer(EventLoop *loop, const struct sockaddr_in &listenAddr);

    ~FileServer();

    void start();

private:
    void onConnection(const TcpConnectionPtr &conn);
    void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time);


    TcpServer tcpServer_;
};


#endif //MINI_MUDUO_FILESERVER_H
