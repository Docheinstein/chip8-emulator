#ifdef CURSES_ENABLED

#ifndef NCDISPLAY_H
#define NCDISPLAY_H

#include <memory>
#include "display.h"
#include "ncwindow.h"
#include "constants.h"

class NcDisplay : public Display {
public:
    explicit NcDisplay(const std::shared_ptr<NcWindow> &window, int horizontal_scaling, int vertical_scaling, bool frame);
    ~NcDisplay() override;

    [[nodiscard]] bool get(unsigned int x, unsigned int y) const override;
    void set(unsigned int x, unsigned int y, bool on) override;
    void clear() override;
    void refresh() override;

private:
    std::shared_ptr<NcWindow> window;
    bool display[CHIP8_DISPLAY_WIDTH * CHIP8_DISPLAY_HEIGHT] {};

    int horizontal_scaling;
    int vertical_scaling;
    bool frame;
};

#endif // NCDISPLAY_H

#endif // CURSES_ENABLED