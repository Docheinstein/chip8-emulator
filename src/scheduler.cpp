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

std::chrono::time_point<std::chrono::system_clock> Scheduler::ScheduledTask::next_execution_time() const {
    return last_execution_time + period;
}

Scheduler::Scheduler(std::initializer_list<Task> tt) {
    for (const auto &t : tt)
        tasks.emplace_back(t);
}

void Scheduler::start() {
    for (auto &t : tasks)
        t.last_execution_time = std::chrono::system_clock::now();
}

void Scheduler::execute_next() {
    ScheduledTask *nearest_task = nullptr;

    for (auto &task : tasks) {
        if (!nearest_task || task.next_execution_time() < nearest_task->next_execution_time())
            nearest_task = &task;
    }

    if (nearest_task) {
        std::this_thread::sleep_until(nearest_task->next_execution_time());
        nearest_task->last_execution_time = std::chrono::system_clock::now();
        (*nearest_task)();
    }
}