#ifdef OPENGL_ENABLED

#include "glwindow.h"
#include "GLFW/glfw3.h"
#include <stdexcept>

GlWindow::GlWindow(int width, int height, const char *name) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    auto W = static_cast<int>(width);
    auto H = static_cast<int>(height);

    glfw_window = glfwCreateWindow(W, H, name, nullptr, nullptr);
    if (!glfw_window) {
        throw std::runtime_error("glfwCreateWindow failed");
    }

    glfwMakeContextCurrent(glfw_window);

    glViewport(0, 0, W, H);
}

GlWindow::~GlWindow() {
    glfwTerminate();
}

GlWindow &GlWindow::operator=(GlWindow &&other) noexcept {
    glfw_window = other.glfw_window;
    return *this;
}

GlWindow::GlWindow(GlWindow &&other) noexcept {
    glfw_window = other.glfw_window;
}


GLFWwindow *GlWindow::window() const {
    return glfw_window;
}

#endif // OPENGL_ENABLED