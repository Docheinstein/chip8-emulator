#ifndef CHIP8_H
#define CHIP8_H

#include <string>
#include "memory.h"
#include "chip8factory.h"
#include "cpu.h"
#include "config.h"
#include "glwindow.h"
#include "gldisplay.h"
#include "glkeypad.h"
#include "powersupply.h"
#include "speaker.h"
#include "eventlooper.h"

class Chip8 {
public:
    explicit Chip8(const Chip8Factory &factory, const Config &cfg);

    bool load(const std::string &filename);
    void run();

private:
    Memory memory;
    std::unique_ptr<Display> display;
    std::unique_ptr<Keypad> keypad;
    std::unique_ptr<PowerSupply> power;
    std::unique_ptr<Speaker> speaker;
    std::unique_ptr<EventLooper> event_looper;
    CPU cpu;

    Config cfg;
};
#endif // CHIP8_H