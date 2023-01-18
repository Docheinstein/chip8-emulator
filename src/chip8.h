#ifndef CHIP8_H
#define CHIP8_H

#include <string>
#include <memory>
#include "cpu.h"
#include "speaker.h"

class Chip8 {
public:
    friend class Chip8Builder;

    bool load(const std::string &rom);
    void run();
    void reset();

private:
    explicit Chip8(int instructions_per_second);

    std::string rom;

    std::unique_ptr<CPU> cpu;
    std::unique_ptr<Memory> memory;
    std::unique_ptr<Display> display;
    std::unique_ptr<Keypad> keypad;
    std::unique_ptr<Speaker> speaker;

    int instructions_per_second;
};

#endif // CHIP8_H