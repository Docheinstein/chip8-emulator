#include "scheduler.h"
#include <thread>

Scheduler::Task::Task(std::chrono::nanoseconds period, void (*task)(void *), void *arg) :
    period(period),
    task(task),
    arg(arg) {}


void Scheduler::Task::operator()() const {
    task(arg);
}

Scheduler::ScheduledTask::ScheduledTask(const Scheduler::Task &task) :
    Task(task),
    last_execution_time() {

}

std::chrono::high_resolution_clock::time_point Scheduler::ScheduledTask::next_execution_time() const {
    return last_execution_time + period;
}

Scheduler::Scheduler(std::initializer_list<Task> tt) {
    for (const auto &t : tt)
        tasks.emplace_back(t);
}

void Scheduler::start() {
    for (auto &t : tasks)
        t.last_execution_time = std::chrono::high_resolution_clock::now();
}

void Scheduler::execute_next() {
    ScheduledTask *nearest_task = nullptr;

    for (auto &task : tasks) {
        if (!nearest_task || task.next_execution_time() < nearest_task->next_execution_time())
            nearest_task = &task;
    }

    if (nearest_task) {
        auto wait_until = nearest_task->next_execution_time();
        while (std::chrono::high_resolution_clock::now() < wait_until); // busy wait
        nearest_task->last_execution_time = std::chrono::high_resolution_clock::now();
        (*nearest_task)();
    }
}