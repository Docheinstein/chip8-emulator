#ifndef DISPLAY_H
#define DISPLAY_H

#include "globals.h"

class Display {
public:
    virtual ~Display() = default;
    bool get(unsigned int x, unsigned int y);
    void set(unsigned int x, unsigned int y, bool on);

    void clear();
    virtual void refresh() = 0;

private:
    bool display[CHIP8_DISPLAY_WIDTH * CHIP8_DISPLAY_HEIGHT] {};
};

#endif // DISPLAY_H