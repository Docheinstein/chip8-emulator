#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <chrono>
#include <vector>

class Scheduler {
public:
    struct Task {
        Task(std::chrono::nanoseconds period, void (*task)(void *), void *arg = nullptr);

        std::chrono::nanoseconds period;
        void (*task)(void *);
        void *arg;

        void operator()() const;
    };

    Scheduler(std::initializer_list<Task> tasks);

    void start();
    void execute_next();

private:
    struct ScheduledTask : public Task {
        explicit ScheduledTask(const Task &task);

        std::chrono::time_point<std::chrono::system_clock> last_execution_time;

        [[nodiscard]] std::chrono::time_point<std::chrono::system_clock> next_execution_time() const;
    };

    std::vector<ScheduledTask> tasks;
};

#endif // SCHEDULER_H