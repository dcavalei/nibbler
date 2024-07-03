#pragma once

#include "game/state/Context.h"

namespace game::state::impl {
    class ExitState : public State {
       public:
        explicit ExitState(Context& context);
        ~ExitState() override = default;

        void handle_event(Event event) override;
    };
}  // namespace game::state::impl
