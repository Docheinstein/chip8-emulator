#ifdef OPENGL_ENABLED

#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include <memory>
#include "display.h"
#include "glwindow.h"
#include "constants.h"

class GlDisplay : public Display {
public:
    explicit GlDisplay(const std::shared_ptr<GlWindow> &window, int horizontal_scaling = 1, int vertical_scaling = 1);

    [[nodiscard]] bool get(unsigned int x, unsigned int y) const override;
    void set(unsigned int x, unsigned int y, bool on) override;
    void clear() override;
    void refresh() override;

private:
    std::shared_ptr<GlWindow> window;
    bool display[CHIP8_DISPLAY_WIDTH * CHIP8_DISPLAY_HEIGHT] {};

    int horizontal_scaling;
    int vertical_scaling;
};

#endif // GLDISPLAY_H

#endif // OPENGL_ENABLED