//
// Created by YngziJ on 2023/12/12.
//

#include "Poller.h"
#include "poller/kqueue.h"


Poller::Poller(EventLoop *loop) : ownerLoop_(loop) {

}

Poller::~Poller() = default;

bool Poller::hasChannel(Channel *channel) const {
    assertInLoopThread();
    auto it = channels_.find(channel->fd());
    return it != channels_.end() && it->second == channel;
}

Poller *Poller::newDefaultPoller(EventLoop *loop) {
    return new Kqueue(loop);
}

