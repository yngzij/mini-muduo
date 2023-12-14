//
// Created by YngziJ on 2023/12/12.
//

#include "EventLoop.h"
#include "Channel.h"
#include "Poller.h"


const int kPollTimeMs = 10000;

EventLoop::EventLoop() : looping_(false),
                         quit_(false),
                         threadId_(CurrentThread::tid()),
                         wakeupFd_(createEventfd()),
                         wakeupChannel_(new Channel(this, wakeupFd_)),
                         callingPendingFunctors_(false),
                         poller_(Poller::newDefaultPoller(this)) {
}


void EventLoop::loop() {
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    quit_ = false;
    while (!quit_) {
        activeChannels_.clear();
        poller_->poll(kPollTimeMs, &activeChannels_);

        for (auto it = activeChannels_.begin(); it != activeChannels_.end(); ++it) {
            (*it)->handleEvent();
        }
        doPendingFunctors();
    }
    looping_ = false;
}

void EventLoop::doPendingFunctors() {
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;
    {
        MutexLockGuard lock(mutex_);
        functors.swap(pendingFunctors_);
    }
    for (const Functor &functor: functors) {
        functor();
    }
    callingPendingFunctors_ = false;
}

void EventLoop::quit() {

}

void EventLoop::wakeup() {

}

void EventLoop::updateChannel(Channel *channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel) {

}

bool EventLoop::hasChannel(Channel *channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    return poller_->hasChannel(channel);
}

EventLoop::~EventLoop() {
    close(wakeupFd_);
}



