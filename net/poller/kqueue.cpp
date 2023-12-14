//
// Created by YngziJ on 2023/12/12.
//

#include <iostream>
#include <sys/event.h>

#include "kqueue.h"
#include "Types.h"


const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;


Kqueue::Kqueue(EventLoop *loop) : Poller(loop), kqueuefd_(::kqueue()), events_(kInitEventListSize) {
    if (kqueuefd_ < 0) {
        assert(false);
    }
}

void Kqueue::removeChannel(Channel *channel) {

}

void Kqueue::updateChannel(Channel *channel) {
    Poller::assertInLoopThread();
    const int index = channel->index();

    if (index == kNew || index == kDeleted) {
        // a new one, add with EPOLL_CTL_ADD
        int fd = channel->fd();
        if (index == kNew) {
            channels_[fd] = channel;
        }
        channel->set_index(kAdded);
        update(EV_ADD, channel);
    } else {
        // update existing one with EPOLL_CTL_MOD/DEL
        int fd = channel->fd();
        (void) fd;
        if (channel->isNoneEvent()) {
            update(EV_DELETE, channel);
            channel->set_index(kDeleted);
        } else {
            update(EV_ENABLE, channel);
        }
    }
}

Timestamp Kqueue::poll(int timeoutMs, Poller::ChannelList *activeChannels) {
    int numEvents = kevent(kqueuefd_, nullptr, 0, &*events_.begin(), static_cast<int>(events_.size()),
                           reinterpret_cast<const timespec *>(timeoutMs));
    Timestamp now(Timestamp::now());
    if (numEvents > 0) {
        fillActiveChannels(numEvents, activeChannels);
        if (static_cast<size_t>(numEvents) == events_.size()) {
            events_.resize(events_.size() * 2);
        }
    } else if (numEvents == 0) {
        // nothing happened
    } else {
        // error happens, log uncommon ones
        if (errno != EINTR) {
            // LOG_SYSERR << "Kqueue::poll()";
        }
    }
    return now;
}

void Kqueue::fillActiveChannels(int numEvents,
                                ChannelList *activeChannels) const {
    assert(implicit_cast<size_t>(numEvents) <= events_.size());
    for (int i = 0; i < numEvents; ++i) {
        Channel *channel = static_cast<Channel *>(events_[i].udata);
#ifndef NDEBUG
        int fd = channel->fd();
        ChannelMap::const_iterator it = channels_.find(fd);
        assert(it != channels_.end());
        assert(it->second == channel);
#endif
        channel->set_revents(events_[i].filter);
        activeChannels->push_back(channel);
    }
}


Kqueue::~Kqueue() {
    close(kqueuefd_);
}

void Kqueue::update(int operation, Channel *channel) {
    struct kevent ev;
    int fd = channel->fd();

    switch (operation) {
        case EV_ADD:
        case EV_ENABLE:
            EV_SET(&ev, fd, EVFILT_READ, operation, 0, 0, channel);
            EV_SET(&ev, fd, EVFILT_WRITE, operation, 0, 0, channel);
            break;

        case EV_DELETE:
            EV_SET(&ev, fd, EVFILT_READ, EV_DELETE, 0, 0, nullptr);
            EV_SET(&ev, fd, EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);
            break;

        default:
            std::cout << "Unsupported operation: " << operation;
            return;
    }

    std::cout << "kevent op = " << operationToString(operation)
              << " fd = " << fd << " event = { " << channel->eventsToString() << " }";
    int ret = kevent(kqueuefd_, &ev, (operation == EV_DELETE) ? 1 : 2, nullptr, 0, nullptr);
    if (ret < 0) {
        if (operation == EV_DELETE) {
            //LOG_SYSERR << "kevent op =" << operationToString(operation) << " fd =" << fd;
        } else {
            //LOG_SYSFATAL << "kevent op =" << operationToString(operation) << " fd =" << fd;
        }
    }
}

const char *Kqueue::operationToString(int op) {
    switch (op) {
        case EV_ADD:
            return "ADD";
        case EV_DELETE:
            return "DEL";
        case EV_ENABLE:
            return "MOD";
        default:
            assert(false && "ERROR op");
            return "Unknown Operation";
    }
}