#ifdef CURSES_ENABLED

#ifndef NCEVENTLOOPER_H
#define NCEVENTLOOPER_H

#include <memory>
#include "eventlooper.h"
#include "ncwindow.h"

class NcEventLooper : public EventLooper {
public:
    explicit NcEventLooper(const std::shared_ptr<NcWindow> &window);
    void poll() override;

private:
    std::shared_ptr<NcWindow> window;
};

#endif // NCEVENTLOOPER_H

#endif // CURSES_ENABLED