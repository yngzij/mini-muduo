//
// Created by YngziJ on 2023/12/12.
//

#ifndef UNTITLED8_EVENTLOOP_H
#define UNTITLED8_EVENTLOOP_H


#include <atomic>
#include <csignal>
#include <vector>
#include <dispatch/dispatch.h>
#include <cassert>
#include <memory>

#include "nocopyable.h"
#include "Mutex.h"
#include "Timestamp.h"


class Channel;


class Poller;


class EventLoop : noncopyable {
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    typedef std::function<void()> Functor;

    void runInLoop(Functor cb);

    bool isInLoopThread() const {
        return threadId_ == CurrentThread::tid();
    }

    // FIXME: pipe is unimplemented
    int createEventfd() {
        int fds[2];
        int ret = pipe(fds);
        assert(ret != -1);
        return fds[1];
    };

    void assertInLoopThread() {
        // threadId 创建EventLoop对象的线程会在构造函数中给threadId_赋值
        // CurrentThread::tid() 当前线程的tid
        // 如果当前线程不是创建EventLoop对象的线程，就会终止程序
        //  isInLoopThread() { return threadId_ == CurrentThread::tid()};
        if (!isInLoopThread()) {
            abortNotInLoopThread();
        } else {
            printf("assertInLoopThread() success\n");
        }
    }

    void abortNotInLoopThread() {
        /*LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
                  << " was created in threadId_ = " << threadId_
                  << ", current thread id = " << CurrentThread::tid();*/

        // 退出程序
    }

    // internal usage
    void wakeup();
    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    bool hasChannel(Channel *channel);

    void doPendingFunctors();
    void queueInLoop(Functor);
private:
    typedef std::vector<Channel *> ChannelList;

    bool looping_; /* atomic */
    std::atomic<bool> quit_;
    const std::thread::id threadId_;
    int wakeupFd_;
    std::unique_ptr<Poller> poller_;
    ChannelList activeChannels_;
    bool callingPendingFunctors_; /* atomic */
    std::vector<Functor> pendingFunctors_ GUARDED_BY(mutex_);
    std::unique_ptr<Channel> wakeupChannel_;
    Timestamp pollReturnTime_;
    Channel *currentActiveChannel_;
    mutable MutexLock mutex_;
    bool eventHandling_;
};


#endif //UNTITLED8_EVENTLOOP_H
