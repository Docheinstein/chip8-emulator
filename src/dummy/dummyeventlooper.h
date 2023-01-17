#ifndef DUMMYEVENTLOOPER_H
#define DUMMYEVENTLOOPER_H

#include "eventlooper.h"

class DummyEventLooper : public EventLooper {
public:
    void poll() override;
};

#endif // DUMMYEVENTLOOPER_H