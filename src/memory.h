#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include "constants.h"

class Memory {
public:
    explicit Memory();
    uint8_t &operator[] (size_t index);
    const uint8_t &operator[] (size_t index) const;

    friend std::ostream & operator <<(std::ostream &, const Memory &);

    void reset();

private:
    uint8_t memory[CHIP8_MEMORY_SIZE] {};
};

#endif // MEMORY_H