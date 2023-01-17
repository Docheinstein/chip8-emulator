#include <algorithm>
#include "dummydisplay.h"

bool DummyDisplay::get(unsigned int x, unsigned int y) const {
    return display[y * CHIP8_DISPLAY_WIDTH + x];
}

void DummyDisplay::set(unsigned int x, unsigned int y, bool on) {
    display[y * CHIP8_DISPLAY_WIDTH + x] = on;
}

void DummyDisplay::clear() {
    std::fill_n(display, sizeof(display) / sizeof(display[0]), false);
}

void DummyDisplay::refresh() {

}
