#ifdef OPENGL_ENABLED

#include <GLFW/glfw3.h>
#include "gldisplay.h"
#include "constants.h"

GlDisplay::GlDisplay(const std::shared_ptr<GlWindow> &window, int horizontal_scaling, int vertical_scaling) :
    window(window), horizontal_scaling(horizontal_scaling), vertical_scaling(vertical_scaling) {

}

bool GlDisplay::get(unsigned int x, unsigned int y) const {
    return display[y  * CHIP8_DISPLAY_WIDTH + x];
}

void GlDisplay::set(unsigned int x, unsigned int y, bool on) {
    display[y  * CHIP8_DISPLAY_WIDTH + x] = on;
}

void GlDisplay::clear() {
    std::fill_n(display, sizeof(display) / sizeof(display[0]), false);
}

void GlDisplay::refresh() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_SCISSOR_TEST);
    for (int y = 0; y < CHIP8_DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < CHIP8_DISPLAY_WIDTH; x++) {
            glScissor(x * horizontal_scaling,
                      (CHIP8_DISPLAY_HEIGHT - y - 1) * vertical_scaling, // OpenGL (0,0) is at bottom
                      horizontal_scaling,
                      vertical_scaling);
            float color = display[y  * CHIP8_DISPLAY_WIDTH + x] ? 1.0f : 0.0f;
            glClearColor(color, color, color, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
    }

    glDisable(GL_SCISSOR_TEST);

    glfwSwapBuffers(window->window());
}
#endif // OPENGL_ENABLED