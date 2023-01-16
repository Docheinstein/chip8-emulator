#include "chip8.h"
#include <fstream>
#include <iostream>
#include "debug.h"
#include "scheduler.h"


Chip8::Chip8(const Chip8Factory &factory, const Config &cfg) :
    display(factory.make_display(cfg.scaling)),
    keypad(factory.make_keypad()),
    power(factory.make_power()),
    speaker(factory.make_speaker()),
    event_looper(factory.make_event_looper()),
    cpu(memory, *display, *keypad, *speaker),
    cfg(cfg) {

}

bool Chip8::load(const std::string &filename) {
    std::ifstream ifs(filename);
    if (ifs) {
        size_t cursor = CHIP8_MEMORY_BEGIN;
        while (ifs)
            memory[cursor++] = ifs.get();
        DBG() << memory << std::endl;
        return true;
    }
    return false;
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
        Scheduler::Task(std::chrono::nanoseconds(1000000000L / CHIP8_TIMERS_TICKS_PER_SECOND), timers_task, &cpu),
        Scheduler::Task(std::chrono::nanoseconds(1000000000L / cfg.instructions_per_seconds), cpu_task, &cpu),
        Scheduler::Task(std::chrono::nanoseconds(1000000000L / CHIP8_DISPLAY_FPS), display_task, &(*display)),
    };

    scheduler.start();

    while (power->on()) {
        scheduler.execute_next();
        event_looper->poll();
    }
}

