#pragma once

#include "game/snake/Snake.h"
#include "game/state/Context.h"

namespace game::state::impl {
    class PlayingState : public State {
        enum class Options { PAUSED, RUNNING, EXIT };

       public:
        explicit PlayingState(Context& context);
        ~PlayingState() override = default;
        void handle_event(Event event) override;

       private:
        void _change_opt(Options opt);
        void _loop();

       private:
        std::unique_ptr<snake::Snake> _snake;
        std::atomic<snake::Orientation> _orientation{snake::Orientation::SOUTH};
        std::atomic<Options> _opt{Options::PAUSED};
        std::mutex _mtx_opt;
        std::condition_variable _cv;
        std::jthread _worker;
    };
}  // namespace game::state::impl
