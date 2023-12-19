//
// Created by YngziJ on 12/18/2023.
//

#include "FileServer.h"
#include "Buffer.h"

#include "TcpConnection.h"


FileServer::FileServer(EventLoop *loop, const struct sockaddr_in &listenAddr)
        : tcpServer_(loop, listenAddr) {
    tcpServer_.setConnectionCallback(std::bind(&FileServer::onConnection, this, std::placeholders::_1));
    tcpServer_.setMessageCallback(std::bind(&FileServer::onMessage, this, std::placeholders::_1, std::placeholders::_2,
                                            std::placeholders::_3));
//    tcpServer_.setWriteCompleteCallback(std::bind(&FileServer::onWriteComplete, this, std::placeholders::_1));

}

FileServer::~FileServer() {
}

void FileServer::start() {
    tcpServer_.start();
}

void FileServer::onConnection(const TcpConnectionPtr &conn) {
    printf("FileServer - %s \n", conn->getPeerAddr().c_str());
}

void FileServer::onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time) {
    printf(".................... %s", buf->retrieveAllAsString().c_str());
}
