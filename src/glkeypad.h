#ifndef GLKEYPAD_H
#define GLKEYPAD_H

#include "keypad.h"

class GLFWwindow;

class GlKeypad : public Keypad {
public:
    explicit GlKeypad(GLFWwindow *window);
    ~GlKeypad() override;

    KeyState get(int key) override;

private:
    GLFWwindow *window;
};

#endif // GLKEYPAD_H