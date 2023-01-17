#ifdef OPENGL_ENABLED

#ifndef GLEVENTLOOPER_H
#define GLEVENTLOOPER_H

#include <memory>
#include "eventlooper.h"
#include "glwindow.h"

class GlEventLooper : public EventLooper {
public:
    explicit GlEventLooper(const std::shared_ptr<GlWindow> &window);
    void poll() override;

private:
    std::shared_ptr<GlWindow> window;
};

#endif // GLEVENTLOOPER_H

#endif // OPENGL_ENABLED