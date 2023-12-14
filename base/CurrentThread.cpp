//
// Created by YngziJ on 2023/12/12.
//

#include "CurrentThread.h"

thread_local std::thread::id CurrentThread::t_cachedTid = std::thread::id();

void CurrentThread::cacheTid() {
    if (t_cachedTid == std::thread::id()) {
        t_cachedTid = std::this_thread::get_id();
    }
}