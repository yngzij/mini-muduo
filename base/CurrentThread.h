//
// Created by YngziJ on 2023/12/12.
//

#ifndef UNTITLED8_CURRENTTHREAD_H
#define UNTITLED8_CURRENTTHREAD_H


#include <thread>


namespace CurrentThread {
    extern thread_local std::thread::id t_cachedTid;

    void cacheTid();

    inline std::thread::id tid() {
            if (t_cachedTid == std::thread::id()) {
            cacheTid();
        }
        return t_cachedTid;
    }
}


#endif //UNTITLED8_CURRENTTHREAD_H
