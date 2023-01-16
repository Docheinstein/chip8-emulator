#include "glwindow.h"


GlWindow::GlWindow(int width, int height, const char *name) :
    width(width),
    height(height),
    name(name),
    window(nullptr) {

}

void GlWindow::init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    auto W = static_cast<int>(width);
    auto H = static_cast<int>(height);

    window = glfwCreateWindow(W, H, name, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    glViewport(0, 0, W, H);
}



