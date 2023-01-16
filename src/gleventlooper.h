#ifndef GLEVENTLOOPER_H
#define GLEVENTLOOPER_H

#include "eventlooper.h"

class GlEventLooper : public EventLooper {
public:
    explicit GlEventLooper();
    void poll() override;
};

#endif // GLEVENTLOOPER_H