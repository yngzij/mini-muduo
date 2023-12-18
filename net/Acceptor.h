//
// Created by YngziJ on 12/18/2023.
//

#ifndef MINI_MUDUO_ACCEPTOR_H
#define MINI_MUDUO_ACCEPTOR_H

class EventLoop;

class Channel;

class Acceptor {
public:
    typedef std::function<void(int sockfd, const struct sockaddr_in &)> NewConnectionCallback;

    Acceptor(EventLoop *loop, const struct sockaddr_in &listenAddr);

    ~Acceptor();

    void setNewConnectionCallback(const NewConnectionCallback &cb) {
        newConnectionCallback_ = cb;
    }

    bool listenning() const {
        return listenning_;
    }

    void listen();

    Acceptor(EventLoop *loop, const struct sockaddr_in &listenAddr);

    ~Acceptor();

    void setNewConnectionCallback(const NewConnectionCallback &cb) {
        newConnectionCallback_ = cb;
    }

    bool listenning() const {
        return listenning_;
    }

    void listen();

private:
    void handleRead();

    EventLoop *loop_;
    Channel acceptChannel_;
    NewConnectionCallback newConnectionCallback_;
    bool listening_;
    int idleFd_;
};


#endif //MINI_MUDUO_ACCEPTOR_H
