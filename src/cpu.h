#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <random>
#include <optional>
#include "memory.h"
#include "register.h"
#include "display.h"
#include "keypad.h"
#include "speaker.h"
#include "constants.h"

typedef Register<uint8_t> Register8;
typedef Register<uint16_t> Register16;

class CPU {
public:
    explicit CPU(Memory &memory, Display &display, Keypad &keypad, Speaker &speaker);
    void step();
    void tick();
    void reset();

    friend std::ostream & operator <<(std::ostream &, const CPU &);

private:
    Register16 PC {"PC"};
    Register16 SP {"SP"};
    Register16 I {"I"};

    Register8 DT {"DT"};
    Register8 ST {"ST"};

    Register8 V[CHIP8_REGISTERS] = {
        Register8("V0"),
        Register8("V1"),
        Register8("V2"),
        Register8("V3"),
        Register8("V4"),
        Register8("V5"),
        Register8("V6"),
        Register8("V7"),
        Register8("V8"),
        Register8("V9"),
        Register8("VA"),
        Register8("VB"),
        Register8("VC"),
        Register8("VD"),
        Register8("VE"),
        Register8("VF"),
    };
    uint16_t stack[CHIP8_STACK_SIZE] {};

    Memory &memory;
    Display &display;
    Keypad &keypad;
    Speaker &speaker;

    std::random_device random_device;
    std::default_random_engine random_engine;
    std::uniform_int_distribution<int> random_uint8_generator;

    struct PendingLdk {
        explicit PendingLdk(Register8 &reg);
        PendingLdk &operator =(const PendingLdk &other);
        Register8 &reg;
        std::optional<int> key_pressed;
    };

    std::optional<PendingLdk> pending_ldk;

    uint8_t rnd();

    void execute_addc(Register8 &reg1, const Register8 &reg2);
    template<typename T1, typename T2>
    void execute_add(Register<T1> &reg1, const Register<T2> &reg2);
    template<typename T>
    void execute_add(Register<T> &reg, T value);
    void execute_and(Register8 &reg1, const Register8 &reg2);
    void execute_bcd(Register<uint16_t> &reg1, const Register8 &reg2);
    void execute_call(uint16_t addr);
    void execute_cls();
    void execute_drw(const Register8 &regx, const Register8 &regy, uint8_t n);
    void execute_jmp(uint16_t addr);
    void execute_jmpV0(uint16_t addr);
    void execute_ldfnt(Register16 &reg1, const Register8 &reg2);
    void execute_ldk(Register8 &reg);
    void execute_ld(Register8 &reg1, const Register8 &reg2);
    template<typename T>
    void execute_ld(Register<T> &reg, T value);
    void execute_ldx(Register<uint16_t> &reg1, uint8_t x);
    void execute_or(Register8 &reg1, const Register8 &reg2);
    void execute_ret();
    void execute_rnd(Register8 &reg, uint8_t value);
    void execute_se(const Register8 &reg1, const Register8 &reg2);
    void execute_se(const Register8 &reg, uint8_t value);
    void execute_shl(Register8 &reg1, const Register8 &reg2);
    void execute_shr(Register8 &reg1, const Register8 &reg2);
    void execute_sknp(const Register<uint8_t> &reg);
    void execute_skp(const Register<uint8_t> &reg);
    void execute_sne(const Register8 &reg1, const Register8 &reg2);
    void execute_sne(const Register8 &reg, uint8_t value);
    void execute_stx(Register<uint16_t> &reg1, uint8_t x);
    void execute_subn(Register8 &reg1, const Register8 &reg2);
    void execute_sub(Register8 &reg1, const Register8 &reg2);
    void execute_xor(Register8 &reg1, const Register8 &reg2);
};

#endif // CPU_H