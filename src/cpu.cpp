#include "cpu.h"
#include "debug.h"
#include <iostream>
#include <iomanip>
#include <sstream>

CPU::CPU(Memory &memory, Display &display, Keypad &keypad, Speaker &speaker) :
        memory(memory),
        display(display),
        keypad(keypad),
        speaker(speaker),
        random_device(),
        random_engine(random_device()),
        random_uint8_generator(0, 255),
        pending_ldk(std::nullopt) {
    reset();
}

void CPU::reset() {
    PC = CHIP8_MEMORY_BEGIN;
    SP = 0;
    I = 0;
    DT = 0;
    ST = 0;
    for (auto &v : V)
        v = 0;
    for (auto &r : stack)
        r = 0;
    pending_ldk = std::nullopt;
}

void CPU::step() {
    if (pending_ldk) {
        execute_ldk(pending_ldk->reg);
        return;
    }

    DEBUG(2) << "--------------" << std::endl;
    DEBUG(2) << *this << std::endl;

    // fetch
    auto l = memory[PC++];
    auto r = memory[PC++];
    uint16_t instr = (l << 8) | r;

    DEBUG(2) << "Instruction: " << std::hex << std::setfill('0') << std::setw(2) << (uint16_t) l << " " << std::setw(2) << (uint16_t) r << std::endl;

    uint16_t op = instr >> 12;
    uint8_t x = (instr & 0x0F00) >> 8;
    uint8_t y = (instr & 0x00F0) >> 4;
    uint8_t n = (instr & 0x000F);
    uint8_t nn = (instr & 0x00FF);
    uint16_t nnn = (instr & 0x0FFF);

    std::stringstream err;

    // decode
    switch (op) {
        case 0x0:
            switch (instr) {
                case 0x00E0:
                    execute_cls();
                    break;
                case 0x00EE:
                    execute_ret();
                    break;
                default:
                    goto UNKNOWN_INSTRUCTION;
            }
            break;
        case 0x1:
            execute_jmp(nnn);
            break;
        case 0x2:
            execute_call(nnn);
            break;
        case 0x3:
            execute_se(V[x], nn);
            break;
        case 0x4:
            execute_sne(V[x], nn);
            break;
        case 0x5:
            execute_se(V[x], V[y]);
            break;
        case 0x6:
            execute_ld(V[x], nn);
            break;
        case 0x7:
            execute_add(V[x], nn);
            break;
        case 0x8:
            switch (n) {
                case 0x0:
                    execute_ld(V[x], V[y]);
                    break;
                case 0x1:
                    execute_or(V[x], V[y]);
                    break;
                case 0x2:
                    execute_and(V[x], V[y]);
                    break;
                case 0x3:
                    execute_xor(V[x], V[y]);
                    break;
                case 0x4:
                    execute_addc(V[x], V[y]);
                    break;
                case 0x5:
                    execute_sub(V[x], V[y]);
                    break;
                case 0x6:
                    execute_shr(V[x], V[y]);
                    break;
                case 0x7:
                    execute_subn(V[x], V[y]);
                    break;
                case 0xE:
                    execute_shl(V[x], V[y]);
                    break;
                default:
                    goto UNKNOWN_INSTRUCTION;
            }
            break;
        case 0x9:
            execute_sne(V[x], V[y]);
            break;
        case 0xA:
            execute_ld(I, nnn);
            break;
        case 0xB:
            execute_jmpV0(nnn);
            break;
        case 0xC:
            execute_rnd(V[x], nn);
            break;
        case 0xD:
            execute_drw(V[x], V[y], n);
            break;
        case 0xE:
            switch (nn) {
                case 0x9E:
                    execute_skp(V[x]);
                    break;
                case 0xA1:
                    execute_sknp(V[x]);
                    break;
                default:
                    goto UNKNOWN_INSTRUCTION;
            }
            break;
        case 0xF:
            switch (nn) {
                case 0x07:
                    execute_ld(V[x], DT);
                    break;
                case 0x0A:
                    execute_ldk(V[x]);
                    break;
                case 0x15:
                    execute_ld(DT, V[x]);
                    break;
                case 0x18:
                    execute_ld(ST, V[x]);
                    break;
                case 0x1E:
                    execute_add(I, V[x]);
                    break;
                case 0x29:
                    execute_ldfnt(I, V[x]);
                    break;
                case 0x33:
                    execute_bcd(I, V[x]);
                    break;
                case 0x55:
                    execute_stx(I, x);
                    break;
                case 0x65:
                    execute_ldx(I, x);
                    break;
                default:
                    goto UNKNOWN_INSTRUCTION;
            }
            break;
        default:
            goto UNKNOWN_INSTRUCTION;
    }

    return;

UNKNOWN_INSTRUCTION:
    err << std::hex << std::setfill('0') << std::setw(2) << (uint16_t) l << " " << std::setw(2) << (uint16_t) r;
    throw std::runtime_error("unknown instruction: " + err.str());
}

void CPU::execute_cls() {
    DEBUG(2) << "CLS" << std::endl;
    display.clear();
}

void CPU::execute_jmp(uint16_t addr) {
    DEBUG(2) << "JMP(0x" << std::hex << std::setfill('0') << std::setw(4) << addr << ")" << std::endl;
    PC = addr;
}

void CPU::execute_jmpV0(uint16_t addr) {
    DEBUG(2) << "JMPv0(0x" << std::hex << std::setfill('0') << std::setw(4) << addr << ")" << std::endl;
    PC = *V[0x0] + addr;
}

void CPU::execute_call(uint16_t addr) {
    DEBUG(2) << "CALL(0x" << std::hex << std::setfill('0') << std::setw(4) << addr << ")" << std::endl;
    stack[SP++] = *PC;
    PC = addr;
}

void CPU::execute_ret() {
    DEBUG(2) << "RET" << std::endl;
    PC = stack[--SP];
}

void CPU::execute_se(const Register8 &reg, uint8_t value) {
    DEBUG(2) << "SE(" << reg << "," << std::dec << (uint16_t) value << ")" << std::endl;
    if (*reg == value)
        PC += 2;
}

void CPU::execute_se(const Register8 &reg1, const Register8 &reg2) {
    DEBUG(2) << "SE(" << reg1 << "," << reg2 << ")" << std::endl;
    if (*reg1 == *reg2)
        PC += 2;
}

void CPU::execute_sne(const Register8 &reg, uint8_t value) {
    DEBUG(2) << "SNE(" << reg << "," << std::dec << (uint16_t) value << ")" << std::endl;
    if (*reg != value)
        PC += 2;
}

void CPU::execute_sne(const Register8 &reg1, const Register8 &reg2) {
    DEBUG(2) << "SNE(" << reg1 << "," << reg2 << ")" << std::endl;
    if (*reg1 != *reg2)
        PC += 2;
}


void CPU::execute_skp(const Register<uint8_t> &reg) {
    DEBUG(2) << "SKP(" << reg << ")" << std::endl;
    if (keypad.get(*reg) == Keypad::KEY_STATE_PRESSED)
        PC += 2;
}

void CPU::execute_sknp(const Register<uint8_t> &reg) {
    DEBUG(2) << "SKNP(" << reg << ")" << std::endl;
    if (keypad.get(*reg) == Keypad::KEY_STATE_RELEASED)
        PC += 2;
}

template<typename T>
void CPU::execute_ld(Register<T> &reg, T value) {
    DEBUG(2) << "LD(" << reg << "," << std::dec << (uint16_t) value << ")" << std::endl;
    reg = value;
}

void CPU::execute_ld(Register8 &reg1, const Register8 &reg2) {
    DEBUG(2) << "LD(" << reg1 << "," << reg2 << ")" << std::endl;
    reg1 = *reg2;
}

void CPU::execute_or(Register8 &reg1, const Register8 &reg2) {
    DEBUG(2) << "OR(" << reg1 << "," << reg2 << ")" << std::endl;
    reg1 = *reg1 | *reg2;
}

void CPU::execute_and(Register8 &reg1, const Register8 &reg2) {
    DEBUG(2) << "AND(" << reg1 << "," << reg2 << ")" << std::endl;
    reg1 = *reg1 & *reg2;
}

void CPU::execute_xor(Register8 &reg1, const Register8 &reg2) {
    DEBUG(2) << "XOR(" << reg1 << "," << reg2 << ")" << std::endl;
    reg1 = *reg1 ^ *reg2;
}

template<typename T1, typename T2>
void CPU::execute_add(Register<T1> &reg1, const Register<T2> &reg2) {
    DEBUG(2) << "ADD(" << reg1 << "," << reg2 << ")" << std::endl;
    reg1 = *reg1 + *reg2;
}

void CPU::execute_addc(Register8 &reg1, const Register8 &reg2) {
    DEBUG(2) << "ADDC(" << reg1 << "," << reg2 << ")" << std::endl;
    uint32_t result = *reg1 + *reg2;
    V[0xF] = (result > 255) ? 1 : 0;
    reg1 = result & 0xFFFF;
}

void CPU::execute_sub(Register8 &reg1, const Register8 &reg2) {
    DEBUG(2) << "SUB(" << reg1 << "," << reg2 << ")" << std::endl;
    uint8_t result = *reg1 - *reg2;
    V[0xF] = (*reg2 > *reg1) ? 0 : 1;
    reg1 = result;
}

void CPU::execute_subn(Register8 &reg1, const Register8 &reg2) {
    DEBUG(2) << "SUBN(" << reg1 << "," << reg2 << ")" << std::endl;
    uint8_t result = *reg2 - *reg1;
    V[0xF] = (*reg1 > *reg2) ? 0 : 1;
    reg1 = result;
}

void CPU::execute_shr(Register8 &reg1, const Register8 &reg2) {
    DEBUG(2) << "SHR(" << reg1 << "," << reg2 << ")" << std::endl;
    // Ambiguous instruction!
    auto flag = (*reg1 & 0x1);
    reg1 = (*reg1 >> 1);
    V[0xF] = flag;
}

void CPU::execute_shl(Register8 &reg1, const Register8 &reg2) {
    DEBUG(2) << "SHL(" << reg1 << "," << reg2 << ")" << std::endl;
    // Ambiguous instruction!
    auto flag = ((*reg1 >> 7) & 0x1);
    reg1 = (*reg1 << 1);
    V[0xF] = flag;
}

template<typename T>
void CPU::execute_add(Register<T> &reg, T value) {
    DEBUG(2) << "ADD(" << reg << "," << std::dec << (uint16_t) value << ")" << std::endl;
    reg = *reg + value;
}


void CPU::execute_drw(const Register8 &regx, const Register8 &regy, uint8_t n) {
    DEBUG(2) << "DRW(" << regx << "," << regy << "," << std::dec << (uint16_t) n << ")" << std::endl;
    uint8_t x = (*regx) % CHIP8_DISPLAY_WIDTH;
    uint8_t y = (*regy) % CHIP8_DISPLAY_HEIGHT;
    uint16_t addr = *I;
    V[0xF] = 0;

    for (int i = 0; i < n; i++, addr++) {
        auto yy = y + i;
        if (yy >= CHIP8_DISPLAY_HEIGHT) {
            break;
        }
        uint8_t byte = memory[addr];
        for (int b = 0; b < 8; b++) {
            auto xx = x + b;
            if (xx >= CHIP8_DISPLAY_WIDTH) {
                break;
            }
            auto _1 = (byte >> (7 - b)) & 0x1;
            auto _2 = display.get(xx, yy) ? 1 : 0;
            auto on = _1 ^ _2;
            if (_2 && !on)
                V[0xF] = 1;
            display.set(xx, yy, on);
        }
    }
}

void CPU::execute_rnd(Register8 &reg, uint8_t value) {
    DEBUG(2) << "RND(" << reg  << "," << std::dec << (uint16_t) value << ")" << std::endl;
    uint8_t tmp = rnd();
    reg = tmp & value;
}

void CPU::execute_ldfnt(Register16 &reg1, const Register8 &reg2) {
    DEBUG(2) << "LDFNT(" << reg1 << "," << reg2 << ")" << std::endl;
    auto fnt = *reg2;
    reg1 = 5 * fnt;
}

void CPU::execute_ldx(Register<uint16_t> &reg, uint8_t x) {
    DEBUG(2) << "LDX(" << reg  << "," << std::dec << (uint16_t) x << ")" << std::endl;
    auto addr = *reg;
    for (int i = 0; i <= x; i++) {
        V[i] = memory[addr + i];
    }
}

void CPU::execute_stx(Register<uint16_t> &reg, uint8_t x) {
    DEBUG(2) << "STX(" << reg  << "," << std::dec << (uint16_t) x << ")" << std::endl;
    auto addr = *reg;
    for (int i = 0; i <= x; i++) {
        memory[addr + i] = *V[i];
    }
}

void CPU::execute_ldk(Register8 &reg) {
    DEBUG(2) << "LDK(" << reg << ")" << std::endl;

    if (pending_ldk && pending_ldk->key_pressed) {
        if (keypad.get(*pending_ldk->key_pressed) == Keypad::KEY_STATE_RELEASED) {
            reg = *pending_ldk->key_pressed;
            pending_ldk = std::nullopt;
        }
    } else {
        if (!pending_ldk)
            pending_ldk = PendingLdk(reg);
        for (int k = 0; k <= 0xF; k++) {
            if (keypad.get(k) == Keypad::KEY_STATE_PRESSED) {
                pending_ldk->key_pressed = k;
                return;
            }
        }
    }
}

void CPU::execute_bcd(Register<uint16_t> &reg1, const Register8 &reg2) {
    DEBUG(2) << "BCD(" << reg1 << "," << reg2 << ")" << std::endl;
    uint16_t addr = *reg1;
    auto h = (*reg2) / 100;
    auto d = ((*reg2) % 100) / 10;
    auto u = (*reg2) % 10;
    memory[addr] = h;
    memory[addr + 1] = d;
    memory[addr + 2] = u;
}

uint8_t CPU::rnd() {
    return random_uint8_generator(random_engine);
}

void CPU::tick() {
    if (*DT)
        --DT;
    if (*ST)
        --ST;
    if (*ST > 0) {
        speaker.play();
    } else {
        speaker.stop();
    }
}

std::ostream &operator<<(std::ostream &os, const CPU &cpu) {
    os  << " PC  "
        << " SP  "
        << " I   "
        << "D  "
        << "S  "
        << "V0 "
        << "V1 "
        << "V2 "
        << "V3 "
        << "V4 "
        << "V5 "
        << "V6 "
        << "V7 "
        << "V8 "
        << "V9 "
        << "VA "
        << "VB "
        << "VC "
        << "VD "
        << "VE "
        << "VF "
        << "\n"
        << std::setfill('0')
        << std::hex
        << std::setw(4) << *(cpu.PC) << " "
        << std::setw(4) << *(cpu.SP) << " "
        << std::setw(4) << *(cpu.I) << " "
        << std::setw(2) << (uint16_t) *(cpu.DT) << " "
        << std::setw(2) << (uint16_t) *(cpu.ST) << " "
        << std::setw(2) << (uint16_t) *(cpu.V[0x0]) << " "
        << std::setw(2) << (uint16_t) *(cpu.V[0x1]) << " "
        << std::setw(2) << (uint16_t) *(cpu.V[0x2]) << " "
        << std::setw(2) << (uint16_t) *(cpu.V[0x3]) << " "
        << std::setw(2) << (uint16_t) *(cpu.V[0x4]) << " "
        << std::setw(2) << (uint16_t) *(cpu.V[0x5]) << " "
        << std::setw(2) << (uint16_t) *(cpu.V[0x6]) << " "
        << std::setw(2) << (uint16_t) *(cpu.V[0x7]) << " "
        << std::setw(2) << (uint16_t) *(cpu.V[0x8]) << " "
        << std::setw(2) << (uint16_t) *(cpu.V[0x9]) << " "
        << std::setw(2) << (uint16_t) *(cpu.V[0xA]) << " "
        << std::setw(2) << (uint16_t) *(cpu.V[0xB]) << " "
        << std::setw(2) << (uint16_t) *(cpu.V[0xC]) << " "
        << std::setw(2) << (uint16_t) *(cpu.V[0xD]) << " "
        << std::setw(2) << (uint16_t) *(cpu.V[0xE]) << " "
        << std::setw(2) << (uint16_t) *(cpu.V[0xF]);
    return os;
}

CPU::PendingLdk::PendingLdk(Register8 &reg) :
    reg(reg),
    key_pressed() {

}

CPU::PendingLdk &CPU::PendingLdk::operator=(const CPU::PendingLdk &other) {
    if (this == &other)
        return *this;
    reg = other.reg;
    key_pressed = other.key_pressed;
    return *this;
}
