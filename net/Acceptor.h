//
// Created by YngziJ on 12/18/2023.
//

#ifndef MINI_MUDUO_ACCEPTOR_H
#define MINI_MUDUO_ACCEPTOR_H


#include <memory>
#include <functional>

#include "nocopyable.h"
#include "Channel.h"


class EventLoop;


class Acceptor : noncopyable {
public:
    Acceptor(EventLoop *loop, const struct sockaddr_in &listenAddr);

    ~Acceptor();

    typedef std::function<void(int sockfd, const struct sockaddr_in &)> NewConnectionCallback;


    void listen();

    bool listening() const { return listening_; }


    void setNewConnectionCallback(const NewConnectionCallback &cb) {
        newConnectionCallback_ = cb;
    }

private:
    void handleRead();
    int acceptSocket_;
    Channel acceptChannel_;
    EventLoop *loop_;
    std::unique_ptr<Acceptor> acceptor_; // avoid revealing Acceptor
    NewConnectionCallback newConnectionCallback_;
    bool listening_;
    int idleFd_;

};


#endif //MINI_MUDUO_ACCEPTOR_H
