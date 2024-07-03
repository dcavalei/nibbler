//
// Created by dcavalei on 25-06-2024.
//

#include "Worker.h"

namespace game::worker {
    Worker::Worker(size_t n) {
        _pool.reserve(n);
        for (size_t i = 0; i < n; i++) {
            _pool.emplace_back(&Worker::_wait_task, this, i);
        }
    }

    Worker::~Worker() {
        _exit = true;
        _cv_wait.notify_all();
    }

    void Worker::push_task(Worker::Task&& task) {
        _mtx_task.lock();
        _task = task;
        _cv_wait.notify_one();
    }

    void Worker::_wait_task([[maybe_unused]] int id) {
        SPDLOG_DEBUG("Hello from worker thread [{}]", id);

        while (true) {
            std::unique_lock lk_wait(_mtx_wait);
            _cv_wait.wait(lk_wait, [this] { return _exit || _task; });
            if (_exit) return;

            auto task = Task();
            {
                std::unique_lock lk_task(_mtx_task, std::adopt_lock);
                task = std::move(_task);
                _task = nullptr;
            }
            lk_wait.unlock();
            task();
        }

        SPDLOG_DEBUG("Goodbye from worker thread [{}]", id);
    }

}  // namespace game::worker
