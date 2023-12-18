//
// Created by YngziJ on 2023/12/12.
//

#include "Channel.h"

#include "EventLoop.h"
#include <sys/event.h>

#include <string>
#include <sstream>


const int Channel::kNoneEvent = 0;
// FIXME: use EPOLLRDHUP instead of EPOLLHUP
const int Channel::kReadEvent = EVFILT_READ;
const int Channel::kWriteEvent = EVFILT_WRITE;


Channel::Channel(EventLoop *loop, int fd__) : loop_(loop),
                                              fd_(fd__),
                                              events_(0),
                                              revents_(0),
                                              index_(-1),
                                              logHup_(true),
                                              tied_(false),
                                              eventHandling_(false),
                                              addedToLoop_(false) {
}

void Channel::handleEvent(Timestamp receiveTime) {
    std::shared_ptr<void> guard;
    if (tied_) {
        guard = tie_.lock();
        if (guard) {
            handleEventWithGuard(receiveTime);
        }
    } else {
        handleEventWithGuard(receiveTime);
    }
}

Channel::~Channel() {
    assert(!eventHandling_);
    assert(!addedToLoop_);
    if (loop_->isInLoopThread()) {
        assert(!loop_->hasChannel(this));
    }
}

void Channel::handleEventWithGuard(Timestamp receiveTime) {
    eventHandling_ = true;
    if ((revents_ & EVFILT_READ) && readCallback_) {
        readCallback_(receiveTime);
    }
    if ((revents_ & EVFILT_WRITE) && writeCallback_) {
        writeCallback_();
    }

    eventHandling_ = false;
}

void Channel::update() {
    addedToLoop_ = true;
    loop_->updateChannel(this);
}

std::string Channel::eventsToString() const {
    return eventsToString(fd_, events_);
}

std::string Channel::eventsToString(int fd, int ev) {
    std::ostringstream oss;
    oss << fd << ": ";
    if (ev & EVFILT_READ)
        oss << "IN ";
    if (ev & EVFILT_WRITE)
        oss << "PRI ";
    if (ev & EVFILT_EXCEPT)
        oss << "OUT ";
    if (ev & EVFILT_AIO)
        oss << "HUP ";
    if (ev & EVFILT_VNODE)
        oss << "RDHUP ";
    if (ev & EVFILT_PROC)
        oss << "ERR ";
    if (ev & EVFILT_SIGNAL)
        oss << "NVAL ";

    return oss.str();
}