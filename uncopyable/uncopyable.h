#ifndef _NONCOPYABLE_H
#define _NONCOPYABLE_H

class noncopyable {
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;
protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

#endif _NONCOPYABLE_H
