#ifndef DUMMYDISPLAY_H
#define DUMMYDISPLAY_H

#include "display.h"
#include "constants.h"

class DummyDisplay : public Display {
public:
    [[nodiscard]] bool get(unsigned int x, unsigned int y) const override;
    void set(unsigned int x, unsigned int y, bool on) override;
    void clear() override;
    void refresh() override;
private:
        bool display[CHIP8_DISPLAY_WIDTH * CHIP8_DISPLAY_HEIGHT] {};
};

#endif // DUMMYDISPLAY_H