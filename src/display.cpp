#include "display.h"
#include "debug.h"
#include <algorithm>

bool Display::get(unsigned int x, unsigned int y) {
    return display[y  * CHIP8_DISPLAY_WIDTH + x];
}

void Display::set(unsigned int x, unsigned int y, bool on) {
    display[y * CHIP8_DISPLAY_WIDTH + x] = on;
}

void Display::clear() {
    std::fill_n(display, sizeof(display) / sizeof(display[0]), false);
}