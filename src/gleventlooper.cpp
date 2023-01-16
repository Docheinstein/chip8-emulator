#include "gleventlooper.h"
#include "GLFW/glfw3.h"

GlEventLooper::GlEventLooper() = default;

void GlEventLooper::poll() {
    glfwPollEvents();
}
