#ifdef OPENGL_ENABLED

#include "gleventlooper.h"
#include <GLFW/glfw3.h>
#include "flags.h"

GlEventLooper::GlEventLooper(const std::shared_ptr <GlWindow> &window) : window(window) {

}

void GlEventLooper::poll() {
    glfwPollEvents();
    if (glfwWindowShouldClose(window->window()))
        shutdown_flag = true;
}

#endif // OPENGL_ENABLED