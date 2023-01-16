#include <iomanip>
#include "memory.h"
#include "debug.h"

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

Memory::Memory() = default;
