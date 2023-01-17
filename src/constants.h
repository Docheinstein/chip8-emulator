#ifndef CONSTANTS_H
#define CONSTANTS_H

constexpr int CHIP8_REGISTERS = 16;
constexpr int CHIP8_KEYPAD_KEYS = 16;

constexpr int CHIP8_DISPLAY_FPS = 60;
constexpr int CHIP8_DISPLAY_WIDTH = 64;
constexpr int CHIP8_DISPLAY_HEIGHT = 32;

constexpr int CHIP8_MEMORY_SIZE = 4096;
constexpr int CHIP8_MEMORY_BEGIN = 0x200;
constexpr int CHIP8_TIMERS_TICKS_PER_SECOND = 60;
constexpr int CHIP8_STACK_SIZE = 16;

#endif // CONSTANTS_H