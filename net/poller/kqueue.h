//
// Created by YngziJ on 2023/12/12.
//

#ifndef UNTITLED8_KQUEUE_H
#define UNTITLED8_KQUEUE_H


#include <sys/event.h>

#include "../Poller.h"


class Kqueue : public Poller {
public:
    Kqueue(EventLoop *loop);
    ~Kqueue() override;

    void update(int operation, Channel *channel);

    Timestamp poll(int timeoutMs, ChannelList *activeChannels)
    override;
    void fillActiveChannels(int numEvents,
                            ChannelList *activeChannels) const;

    void updateChannel(Channel *channel)
    override;
    void removeChannel(Channel *channel)
    override;

private:
    static const int kInitEventListSize = 16;
    int kqueuefd_;
    typedef std::vector<struct kevent> EventList;
    EventList events_;
    const char *operationToString(int op);
};


#endif //UNTITLED8_KQUEUE_H
