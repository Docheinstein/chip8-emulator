#ifdef OPENGL_ENABLED

#include "glkeypad.h"
#include "debug.h"
#include "flags.h"
#include "GLFW/glfw3.h"

/*
 *     CHIP-8             Keyboard
 *  1 | 2 | 3 | C       1 | 2 | 3 | 4
 *  4 | 5 | 6 | D       Q | W | E | R
 *  7 | 8 | 9 | E       A | S | D | F
 *  A | 0 | B | F       Z | X | C | V
 */

static const int CHIP8_KEYS_TO_GLFW_KEYS[] = {
    GLFW_KEY_X, // 0x0
    GLFW_KEY_1, // 0x1
    GLFW_KEY_2, // 0x2
    GLFW_KEY_3, // 0x3
    GLFW_KEY_Q, // 0x4
    GLFW_KEY_W, // 0x5
    GLFW_KEY_E, // 0x6
    GLFW_KEY_A, // 0x7
    GLFW_KEY_S, // 0x8
    GLFW_KEY_D, // 0x9
    GLFW_KEY_Z, // 0xA
    GLFW_KEY_C, // 0xB
    GLFW_KEY_4, // 0xC
    GLFW_KEY_R, // 0xD
    GLFW_KEY_F, // 0xE
    GLFW_KEY_V, // 0xF
};

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto *keypad = (GlKeypad *) glfwGetWindowUserPointer(window);
    for (int k = 0; k < CHIP8_KEYPAD_KEYS; k++) {
        if (CHIP8_KEYS_TO_GLFW_KEYS[k] == key) {
            DEBUG() << "Key " << std::hex << k << " " << (action == GLFW_RELEASE ? "up" : "down") << std::endl;
            keypad->_set(k, (action == GLFW_RELEASE) ? Keypad::KEY_STATE_RELEASED : Keypad::KEY_STATE_PRESSED);
        }
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        shutdown_flag = true;
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        reset_flag = true;
}

void GlKeypad::_set(int key, Keypad::KeyState state) {
    keys_state[key] = state;
}

GlKeypad::~GlKeypad() = default;

Keypad::KeyState GlKeypad::get(int key) {
    return keys_state[key];
}

GlKeypad::GlKeypad(const std::shared_ptr<GlWindow> &w) :
    window(w) {
    glfwSetWindowUserPointer(window->window(), this);
    glfwSetKeyCallback(window->window(), key_callback);
}

#endif // OPENGL_ENABLED