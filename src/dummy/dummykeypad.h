#ifndef DUMMYKEYPAD_H
#define DUMMYKEYPAD_H

#include "keypad.h"

class DummyKeypad : public Keypad {
public:
    KeyState get(int key) override;
};

#endif // DUMMYKEYPAD_H