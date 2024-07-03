#include "ExitState.h"

namespace game::state::impl {
    ExitState::ExitState(Context& context) : State(context) {}
    void ExitState::handle_event([[maybe_unused]] Event event) { SPDLOG_DEBUG("Ignoring event {}", toString(event)); }
}  // namespace game::state::impl
