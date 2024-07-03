#pragma once

#include <mutex>
#include <queue>

#include "spdlog/spdlog.h"

namespace game::state {
    enum class Event { EXIT, UP, DOWN, LEFT, RIGHT, ENTER, PLUGIN_1, PLUGIN_2, PLUGIN_3 };
    [[maybe_unused]] static constexpr std::string_view toString(Event event) {
        switch (event) {
            case Event::EXIT: return "EXIT";
            case Event::UP: return "UP";
            case Event::DOWN: return "DOWN";
            case Event::LEFT: return "LEFT";
            case Event::RIGHT: return "RIGHT";
            case Event::ENTER: return "ENTER";
            case Event::PLUGIN_1: return "PLUGIN_1";
            case Event::PLUGIN_2: return "PLUGIN_2";
            case Event::PLUGIN_3: return "PLUGIN_3";
            default: return "WTF";
        }
    }

    class EventQueue {
       public:
        using Callback = std::function<void(Event)>;

        EventQueue();
        ~EventQueue();

        void push(Event event);
        void subscribe(Callback&& cb);

       private:
        void _notify(Event event);
        void _await_events();

       private:
        std::atomic<bool> _exit{false};
        std::mutex _mtx;
        std::condition_variable _cv;
        std::vector<Callback> _subs;
        std::vector<Event> _queue;
        std::jthread _worker;  // must be the last
    };
}  // namespace game::state
