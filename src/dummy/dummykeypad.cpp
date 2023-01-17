#include "dummykeypad.h"

Keypad::KeyState DummyKeypad::get(int key) {
    return KEY_STATE_RELEASED;
}