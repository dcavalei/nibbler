#pragma once

#include <spdlog/spdlog.h>

#include <condition_variable>
#include <functional>
#include <thread>
#include <vector>

namespace game::worker {
    class Worker {
       public:
        using Task = std::function<void()>;

        explicit Worker(size_t n);
        ~Worker();

        void push_task(Task&& task);

       private:
        void _wait_task([[maybe_unused]] int id);

       private:
        Task _task;
        std::mutex _mtx_wait;
        std::mutex _mtx_task;
        std::atomic<bool> _exit{false};
        std::condition_variable _cv_wait;
        std::vector<std::jthread> _pool;
    };
}  // namespace game::worker