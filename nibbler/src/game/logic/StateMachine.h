#pragma once

#include <atomic>
#include <mutex>
#include <queue>

#include "spdlog/spdlog.h"

namespace game::logic {
    namespace exception {
        struct StateMachineUnknownStateException : public std::exception {
            [[nodiscard]] const char *what() const noexcept final { return "Unhandled state in StateMachine!"; }
        };
        struct StateMachineLoopAlreadyRunning : public std::exception {
            [[nodiscard]] const char *what() const noexcept final { return "StateMachine loop already initialized!"; }
        };
    }  // namespace exception
    enum class State { MainMenu, Playing, Paused, GameOver, Exit };
    enum class Event { START, EXIT, UP, DOWN, LEFT, RIGHT, ENTER };

    [[maybe_unused]] static std::string_view toString(Event event) {
        switch (event) {
            case Event::START: return "START";
            case Event::EXIT: return "EXIT";
            case Event::UP: return "UP";
            case Event::DOWN: return "DOWN";
            case Event::LEFT: return "LEFT";
            case Event::RIGHT: return "RIGHT";
            case Event::ENTER: return "ENTER";
            default: return "WTF";
        }
    }
    [[maybe_unused]] static std::string_view toString(State event) {
        switch (event) {
            case State::MainMenu: return "MainMenu";
            case State::Playing: return "Playing";
            case State::Paused: return "Paused";
            case State::GameOver: return "gameOver";
            case State::Exit: return "Exit";
            default: return "WTF";
        }
    }

    // Using Enums to represent state, if the code base gets to complex, consider using objects for state
    class StateMachine {
       public:
        State getCurrentState() { return _state; }
        explicit operator State() { return getCurrentState(); }
        bool operator==(State other) { return _state == other; }

        void init() {
            if (!_loop) {
                spdlog::info("StateMachine loop being initialized");
                _loop = std::make_unique<std::jthread>([this]() { this->_awaitEvent(); });
            } else
                throw exception::StateMachineLoopAlreadyRunning();
        }
        void queueEvent(Event event) {
            std::lock_guard lk(_queue_mutex);
            spdlog::debug("[queueEvent] Event {} being queued", toString(event));
            _queue.push(event);
            _cv.notify_one();
        }
        void loop() {
            std::unique_lock lk(_state_mutex);
            _cv.wait(lk, [this]() { return this->_state == State::Exit; });
        }

       private:
        void _awaitEvent() {
            do {
                std::unique_lock lk(_state_mutex);
                Event event;
                _cv.wait(lk, [this, &event]() {
                    std::lock_guard lk(this->_queue_mutex);
                    if (!this->_queue.empty()) {
                        event = this->_queue.front();
                        this->_queue.pop();
                        return true;
                    }
                    return false;
                });
                // We own _state_mutex.
                _changeState(event);
                lk.unlock();
                // we own nothing.
                _cv.notify_one();
                // safe to check _state here because the other _cv user is read-only.
            } while (_state != State::Exit);
        }

        // This function is not thread safe, must be called inside _awaitEvent().
        void _changeState(Event event) {
            switch (_state) {
                case State::MainMenu:
                    switch (event) {
                        case Event::EXIT: _state = State::Exit; break;
                        case Event::UP: break;
                        case Event::DOWN: break;
                        case Event::ENTER: break;
                        default:
                            spdlog::debug("Ignoring Event {} in State {}", toString(event), toString(_state));
                            break;
                    }
                    break;
                case State::Playing: break;
                case State::Paused: break;
                case State::GameOver: break;
                case State::Exit: break;
                default: throw exception::StateMachineUnknownStateException();
            }
        }

       private:
        std::queue<Event> _queue;
        std::mutex _queue_mutex;
        State _state{State::MainMenu};
        std::mutex _state_mutex;
        std::condition_variable _cv;
        std::unique_ptr<std::jthread> _loop;
    };
}  // namespace game::logic
