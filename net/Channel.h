//
// Created by YngziJ on 2023/12/12.
//

#ifndef UNTITLED8_CHANNEL_H
#define UNTITLED8_CHANNEL_H


#include <thread>
#include <string>


#include "../base/nocopyable.h"
#include "../base/Timestamp.h"


class EventLoop;


class Channel : noncopyable {
public:
    Channel(EventLoop *loop, int fd);
    ~Channel();
private:
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;
public:
    typedef std::function<void()> EventCallback;
    typedef std::function<void(Timestamp)> ReadEventCallback;

    void setReadCallback(ReadEventCallback cb) { readCallback_ = std::move(cb); }

    void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }

    void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }

    void setErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb); }

    void handleEvent(Timestamp timestamp);

    EventLoop *ownerLoop() { return loop_; }


    int index() { return index_; }

    void set_index(int idx) { index_ = idx; }


    int fd() const { return fd_; }

    int events() const { return events_; }

    void set_revents(int revt) { revents_ = revt; } // used by pollers
    // int revents() const { return revents_; }
    bool isNoneEvent() const { return events_ == kNoneEvent; }

    void enableReading() {
        events_ |= kReadEvent;
        update();
    }

    void disableReading() {
        events_ &= ~kReadEvent;
        update();
    }

    void enableWriting() {
        events_ |= kWriteEvent;
        update();
    }

    void disableWriting() {
        events_ &= ~kWriteEvent;
        update();
    }

    void disableAll() {
        events_ = kNoneEvent;
        update();
    }

    bool isWriting() const { return events_ & kWriteEvent; }

    bool isReading() const { return events_ & kReadEvent; }

    std::string eventsToString() const;
private:
    static std::string eventsToString(int fd, int ev);
    void update();
    void handleEventWithGuard(Timestamp receiveTime);


    EventLoop *loop_;
    const int fd_;
    int events_;
    int revents_;
    int index_;
    bool logHup_;
    bool tied_;
    bool eventHandling_;
    bool addedToLoop_;

    std::weak_ptr<void> tie_;

    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;

};


#endif //UNTITLED8_CHANNEL_H
