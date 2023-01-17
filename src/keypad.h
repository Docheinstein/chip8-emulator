#ifndef KEYPAD_H
#define KEYPAD_H

class Keypad {
public:
    enum KeyState {
        KEY_STATE_RELEASED = 0,
        KEY_STATE_PRESSED = 1
    };

    virtual ~Keypad() = default;

    virtual KeyState get(int key) = 0;
};

#endif // KEYPAD_H