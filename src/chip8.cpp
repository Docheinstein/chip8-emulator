#include "chip8.h"
#include <fstream>
#include <iostream>
#include "debug.h"
#include "scheduler.h"
#include "flags.h"

Chip8::Chip8(int instructions_per_second) :
    instructions_per_second(instructions_per_second) {

}


bool Chip8::load(const std::string &rom_) {
    rom = rom_;
    std::ifstream ifs(rom);
    if (ifs) {
        size_t cursor = CHIP8_MEMORY_BEGIN;
        while (ifs)
            (*memory)[cursor++] = ifs.get();
        DEBUG(2) << *memory << std::endl;
        return true;
    }
    return false;
}

void Chip8::reset() {
    DEBUG(1) << "======= RESET ========" << std::endl;
    memory->reset();
    display->clear();
    cpu->reset();
    load(rom);
    DEBUG(2) << *memory << std::endl;
    DEBUG(2) << *cpu << std::endl;
}

void Chip8::run() {
    auto cpu_task = [](void *arg) {
        auto *cpu_ = (CPU *) arg;
        cpu_->step();
    };

    auto timers_task = [](void *arg) {
        auto *cpu_ = (CPU *) arg;
        cpu_->tick();
    };

    auto display_task = [](void *arg) {
        auto *display_ = (Display *) arg;
        display_->refresh();
    };

    Scheduler scheduler {
        Scheduler::Task(std::chrono::nanoseconds(1000000000L / CHIP8_TIMERS_TICKS_PER_SECOND), timers_task, &(*cpu)),
        Scheduler::Task(std::chrono::nanoseconds(1000000000L / instructions_per_second), cpu_task, &(*cpu)),
        Scheduler::Task(std::chrono::nanoseconds(1000000000L / CHIP8_DISPLAY_FPS), display_task, &(*display)),
    };

    scheduler.start();

    while (!shutdown_flag && !reset_flag) {
        scheduler.execute_next();
        event_looper->poll();
    }
}
