#include "glkeypad.h"
#include <GLFW/glfw3.h>

static const int GLFW_KEYS[] = {
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

static Keypad::KeyState KEYS_STATE[0xF + 1];

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    for (int i = 0; i <= 0xF; i++) {
        if (GLFW_KEYS[i] == key) {
            KEYS_STATE[i] = (action == GLFW_RELEASE) ? Keypad::KEY_STATE_RELEASED : Keypad::KEY_STATE_PRESSED;
        }
    }
}


GlKeypad::~GlKeypad() = default;

Keypad::KeyState GlKeypad::get(int key) {
    return KEYS_STATE[key];
}

GlKeypad::GlKeypad(GLFWwindow *win) :
        window(win) {
    glfwSetKeyCallback(window, key_callback);
}
