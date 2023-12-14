//
// Created by YngziJ on 2023/12/12.
//

#ifndef UNTITLED8_POLLER_H
#define UNTITLED8_POLLER_H


#include "nocopyable.h"
#include "Channel.h"
#include "EventLoop.h"

#include <map>
#include <vector>


class Poller : noncopyable {
public:
    typedef std::vector<Channel *> ChannelList;
    Poller(EventLoop *loop);
    virtual ~Poller();

    void assertInLoopThread() const {
        ownerLoop_->assertInLoopThread();
    }


    virtual Timestamp poll(int timeoutMs, ChannelList *activeChannels) = 0;

    /// Changes the interested I/O events.
    /// Must be called in the loop thread.
    virtual void updateChannel(Channel *channel) = 0;

    /// Remove the channel, when it destructs.
    /// Must be called in the loop thread.
    virtual void removeChannel(Channel *channel) = 0;

    virtual bool hasChannel(Channel *channel) const;

    static Poller *newDefaultPoller(EventLoop *loop);
private:
    EventLoop *ownerLoop_;

protected:
    typedef std::map<int, Channel *> ChannelMap;
    ChannelMap channels_;
};


#endif //UNTITLED8_POLLER_H
