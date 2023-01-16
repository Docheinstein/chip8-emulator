#ifndef GLWINDOW_H
#define GLWINDOW_H

#include "GLFW/glfw3.h"

class GlWindow {
public:
    GlWindow(int width, int height, const char *name);

    void init();

    GLFWwindow *window;

private:
    int width;
    int height;
    const char *name;
};


#endif // GLWINDOW_H