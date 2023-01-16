#ifndef EVENTLOOPER_H
#define EVENTLOOPER_H

class EventLooper {
public:
    virtual ~EventLooper() = default;
    virtual void poll() = 0;
};

#endif // EVENTLOOPER_H