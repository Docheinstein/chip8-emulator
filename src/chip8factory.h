#ifndef CHIP8FACTORY_H
#define CHIP8FACTORY_H

#include <memory>
#include "display.h"
#include "keypad.h"
#include "powersupply.h"
#include "speaker.h"
#include "eventlooper.h"

class Chip8Factory {
public:
    virtual ~Chip8Factory() = default;
    [[nodiscard]] virtual std::unique_ptr<Display> make_display(int scaling) const = 0;
    [[nodiscard]] virtual std::unique_ptr<Keypad> make_keypad() const = 0;
    [[nodiscard]] virtual std::unique_ptr<PowerSupply> make_power() const = 0;
    [[nodiscard]] virtual std::unique_ptr<Speaker> make_speaker() const = 0;
    [[nodiscard]] virtual std::unique_ptr<EventLooper> make_event_looper() const = 0;
};

#endif // CHIP8FACTORY_H