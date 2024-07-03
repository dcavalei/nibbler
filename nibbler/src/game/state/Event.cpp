//
// Created by dcavalei on 20-06-2024.
//

#include "Event.h"
namespace game::state {
    EventQueue::EventQueue() : _worker(&EventQueue::_await_events, this) { SPDLOG_INFO("EventQueue starting..."); }

    EventQueue::~EventQueue() {
        _exit = true;
        _cv.notify_one();
    }

    void EventQueue::push(Event event) {
        {
            std::lock_guard lk(_mtx);
            _queue.push_back(event);
            SPDLOG_DEBUG("Event {}", toString(event));
        }
        _cv.notify_one();
    }

    void EventQueue::subscribe(EventQueue::Callback&& cb) {
        SPDLOG_DEBUG("Subscriber 0x{:x}", (std::uintptr_t)&cb);
        _subs.push_back(cb);
    }

    void EventQueue::_notify(Event event) {
        SPDLOG_DEBUG("Notifying {} subscribers...", _subs.size());
        for (auto& cb : _subs) {
            cb(event);
        }
    }

    void EventQueue::_await_events() {
        SPDLOG_INFO("EventQueue [notifier] starting...");
        decltype(_queue) worker_queue;

        while (!_exit) {
            SPDLOG_DEBUG("Waiting for events...");
            std::unique_lock lk(_mtx);
            _cv.wait(lk, [this]() { return _exit || !_queue.empty(); });
            SPDLOG_DEBUG("Wake up...");
            if (_exit) {
                SPDLOG_DEBUG("Exiting...");
                return;
            }

            _queue.swap(worker_queue);
            assert(_queue.empty());
            lk.unlock();

            SPDLOG_DEBUG("Notifying {} events...", worker_queue.size());
            for (auto event : worker_queue) _notify(event);
            worker_queue.clear();
        }
        SPDLOG_INFO("EventQueue [notifier] exiting...");
    }

}  // namespace game::state
