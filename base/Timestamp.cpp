//
// Created by YngziJ on 2023/12/12.
//

#include <sys/time.h>
#include "Timestamp.h"


Timestamp Timestamp::now() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}
