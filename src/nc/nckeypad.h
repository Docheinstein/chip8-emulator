#ifdef CURSES_ENABLED

#ifndef NCKEYPAD_H
#define NCKEYPAD_H

#include <memory>
#include <chrono>
#include "ncwindow.h"
#include "keypad.h"
#include "constants.h"

class NcKeypad : public Keypad {
public:
    explicit NcKeypad(const std::shared_ptr<NcWindow> &window);
    ~NcKeypad() override;

    KeyState get(int key) override;

private:
    std::shared_ptr<NcWindow> window;

    Keypad::KeyState keys_state[CHIP8_KEYPAD_KEYS] {};

    void read_keys();

    int keyboard_fd;
};

#endif // NCKEYPAD_H

#endif // CURSES_ENABLED