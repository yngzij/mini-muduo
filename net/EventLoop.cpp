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
        pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);

        for (auto it = activeChannels_.begin(); it != activeChannels_.end(); ++it) {
            (*it)->handleEvent(pollReturnTime_);
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
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    if (eventHandling_) {
        assert(currentActiveChannel_ == channel ||
               std::find(activeChannels_.begin(), activeChannels_.end(), channel) == activeChannels_.end());
    }
    poller_->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel *channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    return poller_->hasChannel(channel);
}

EventLoop::~EventLoop() {
    close(wakeupFd_);
}

void EventLoop::queueInLoop(Functor cb) {
    {
        MutexLockGuard lock(mutex_);
        pendingFunctors_.push_back(std::move(cb));
    }
    if (!isInLoopThread() || callingPendingFunctors_) {
        wakeup();
    }
}

void EventLoop::runInLoop(EventLoop::Functor cb) {
    // 如果是当前线程调用，直接执行
    if (isInLoopThread()) {
        cb();
        // 如果不是当前线程调用，加入队列，唤醒loop所在线程
    } else {
        queueInLoop(std::move(cb));
    }
}



