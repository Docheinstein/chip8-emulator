#ifdef OPENGL_ENABLED

#ifndef GLKEYPAD_H
#define GLKEYPAD_H

#include <memory>
#include "keypad.h"
#include "glwindow.h"
#include "constants.h"

class GlKeypad : public Keypad {
public:
    explicit GlKeypad(const std::shared_ptr<GlWindow> &window);
    ~GlKeypad() override;

    KeyState get(int key) override;

    void _set(int key, KeyState state);

private:
    std::shared_ptr<GlWindow> window;

    Keypad::KeyState keys_state[CHIP8_KEYPAD_KEYS] {};
};

#endif // GLKEYPAD_H

#endif // OPENGL_ENABLED