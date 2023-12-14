//
// Created by YngziJ on 2023/12/12.
//

#ifndef UNTITLED8_NOCOPYABLE_H
#define UNTITLED8_NOCOPYABLE_H


class noncopyable {
public:
    noncopyable(const noncopyable &) = delete;

    void operator=(const noncopyable &) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};


#endif //UNTITLED8_NOCOPYABLE_H
