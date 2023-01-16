#include <GLFW/glfw3.h>
#include "gldisplay.h"

GlDisplay::GlDisplay(GLFWwindow *window, int scaling) :
        window(window),
        scaling(scaling) {

}

void GlDisplay::refresh() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_SCISSOR_TEST);
    for (int y = 0; y < CHIP8_DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < CHIP8_DISPLAY_WIDTH; x++) {
            glScissor(x * scaling, (CHIP8_DISPLAY_HEIGHT - y - 1) * scaling, scaling, scaling);
            auto color = get(x, y) ? 1.0f : 0.0f;
            glClearColor(color, color, color, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
    }

    glDisable(GL_SCISSOR_TEST);

    glfwSwapBuffers(window);
}
