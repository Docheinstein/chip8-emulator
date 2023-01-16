#ifndef KEYPAD_H
#define KEYPAD_H

class Keypad {
public:
    enum Key {
        KEY_0 = 0x0,
        KEY_1 = 0x1,
        KEY_2 = 0x2,
        KEY_3 = 0x3,
        KEY_4 = 0x4,
        KEY_5 = 0x5,
        KEY_6 = 0x6,
        KEY_7 = 0x7,
        KEY_8 = 0x8,
        KEY_9 = 0x9,
        KEY_A = 0xA,
        KEY_B = 0xB,
        KEY_C = 0xC,
        KEY_D = 0xD,
        KEY_E = 0xE,
        KEY_F = 0xF,
    };

    enum KeyState {
        KEY_STATE_RELEASED = 0,
        KEY_STATE_PRESSED = 1
    };

    virtual ~Keypad() = default;

    virtual KeyState get(int key) = 0;
};

#endif // KEYPAD_H