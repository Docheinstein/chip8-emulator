#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include "display.h"

class GLFWwindow;

class GlDisplay : public Display {
public:
    explicit GlDisplay(GLFWwindow* window, int scaling = 1);

    void refresh() override;

private:
    int scaling;
    GLFWwindow *window;
};

#endif // GLDISPLAY_H