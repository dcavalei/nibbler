#include "PlayingState.h"

#include "game/state/impl/ExitState.h"

namespace game::state::impl {

    PlayingState::PlayingState(Context& context) : State(context), _worker(&PlayingState::_loop, this) {
        try {
            _snake = std::make_unique<snake::Snake>(config().game_width, config().game_height);
        } catch (snake::exception::SnakeSmallMatrixException& e) {
            SPDLOG_CRITICAL("{}", e.what());
            context_change_state<ExitState>();
        }
    }

    void PlayingState::handle_event(Event event) {
        // TODO: can do better...
        switch (event) {
            case Event::EXIT:
                _change_opt(Options::EXIT);
                context_change_state<ExitState>();
                break;
            case Event::UP: _orientation = snake::Orientation::NORTH; break;
            case Event::DOWN: _orientation = snake::Orientation::SOUTH; break;
            case Event::LEFT: _orientation = snake::Orientation::WEST; break;
            case Event::RIGHT: _orientation = snake::Orientation::EAST; break;
            case Event::ENTER:
                switch (_opt) {
                    case Options::PAUSED: _change_opt(Options::RUNNING); break;
                    case Options::RUNNING: _change_opt(Options::PAUSED); break;
                    default:;
                }
                break;
            default: SPDLOG_DEBUG("Ignoring event {}", toString(event));
        }
    }

    void PlayingState::_change_opt(PlayingState::Options opt) {
        _opt = opt;
        _cv.notify_one();
    }

    void PlayingState::_loop() {
        SPDLOG_DEBUG("PLayingState loop start");
        while (_opt != Options::EXIT) {
            std::unique_lock lk(_mtx_opt);

            _cv.wait(lk, [this] { return _opt != Options::PAUSED; });
            if (_opt == Options::EXIT) return;

            try {
                _snake->_player.move(_orientation);
                _snake->_debug();  // TODO IMPROVE

            } catch (snake::exception::PlayerHitException& e) {
                SPDLOG_CRITICAL("{}", e.what());
                _snake->_debug();  // TODO IMPROVE
                return;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(config().game_speed_ms));
        }
    }

}  // namespace game::state::impl
