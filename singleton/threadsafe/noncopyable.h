#ifndef _NONCOPYABLE_H_
#define _NONCOPYABLE_H_

class noncopyable{
  protected:
    noncopyable() = default;
    ~noncopyable() = default;
  private:
    noncopyable(const noncopyable&) = delete;
    const noncopyable& operator=(const noncopyable&) = delete;
};

#endif
