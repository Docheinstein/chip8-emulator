#include <iomanip>
#include "memory.h"
#include "debug.h"


Memory::Memory() {
    reset();
}

void Memory::reset() {
    std::fill_n(memory, sizeof(memory) / sizeof(memory[0]), 0);
    int i = 0;
    memory[i++] = 0xF0; memory[i++] = 0x90; memory[i++] = 0x90; memory[i++] = 0x90; memory[i++] = 0xF0; // 0
    memory[i++] = 0x20; memory[i++] = 0x60; memory[i++] = 0x20; memory[i++] = 0x20; memory[i++] = 0x70; // 1
    memory[i++] = 0xF0; memory[i++] = 0x10; memory[i++] = 0xF0; memory[i++] = 0x80; memory[i++] = 0xF0; // 2
    memory[i++] = 0xF0; memory[i++] = 0x10; memory[i++] = 0xF0; memory[i++] = 0x10; memory[i++] = 0xF0; // 3
    memory[i++] = 0x90; memory[i++] = 0x90; memory[i++] = 0xF0; memory[i++] = 0x10; memory[i++] = 0x10; // 4
    memory[i++] = 0xF0; memory[i++] = 0x80; memory[i++] = 0xF0; memory[i++] = 0x10; memory[i++] = 0xF0; // 5
    memory[i++] = 0xF0; memory[i++] = 0x80; memory[i++] = 0xF0; memory[i++] = 0x90; memory[i++] = 0xF0; // 6
    memory[i++] = 0xF0; memory[i++] = 0x10; memory[i++] = 0x20; memory[i++] = 0x40; memory[i++] = 0x40; // 7
    memory[i++] = 0xF0; memory[i++] = 0x90; memory[i++] = 0xF0; memory[i++] = 0x90; memory[i++] = 0xF0; // 8
    memory[i++] = 0xF0; memory[i++] = 0x90; memory[i++] = 0x90; memory[i++] = 0x90; memory[i++] = 0xF0; // 9
    memory[i++] = 0xF0; memory[i++] = 0x90; memory[i++] = 0xF0; memory[i++] = 0x90; memory[i++] = 0x90; // A
    memory[i++] = 0xE0; memory[i++] = 0x90; memory[i++] = 0xE0; memory[i++] = 0x90; memory[i++] = 0xE0; // B
    memory[i++] = 0xF0; memory[i++] = 0x80; memory[i++] = 0x80; memory[i++] = 0x80; memory[i++] = 0xF0; // C
    memory[i++] = 0xE0; memory[i++] = 0x90; memory[i++] = 0x90; memory[i++] = 0x90; memory[i++] = 0xE0; // D
    memory[i++] = 0xF0; memory[i++] = 0x80; memory[i++] = 0xF0; memory[i++] = 0x80; memory[i++] = 0xF0; // E
    memory[i++] = 0xF0; memory[i++] = 0x80; memory[i++] = 0xF0; memory[i++] = 0x80; memory[i++] = 0x80; // F
}

uint8_t &Memory::operator[](size_t index) {
    return memory[index];
}

const uint8_t &Memory::operator[](size_t index) const {
    return memory[index];
}

std::ostream &operator<<(std::ostream &os, const Memory &mem) {
    static const int COLUMNS = 32;
    for (int i = 0; i < CHIP8_MEMORY_SIZE; i++) {
        if (i % COLUMNS == 0)
            os << std::hex << std::setfill('0') << std::setw(4) << i << " | ";
        auto b = mem[i];
        os << std::hex << std::setfill('0') << std::setw(2) << (uint16_t) b;

        if ((i + 1) % COLUMNS == 0) {
            if ((i + 1) < CHIP8_MEMORY_SIZE)
                os << std::endl;
        } else {
            os << " ";
        }
    }
    return os;
}
