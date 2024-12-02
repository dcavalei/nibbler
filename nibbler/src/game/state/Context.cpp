#include "Context.h"

#include "game/Config.h"
#include "game/state/Event.h"
#include "game/state/impl/MainMenuState.h"
#include "game/state/impl/PlayingState.h"
#include "spdlog/spdlog.h"

namespace game::state {
    Context::Context(game::config::Config& c, plugin::PluginSwitcher& plugin_switcher)
        : _config(c), plugin_switcher(plugin_switcher) {
        _change_state<impl::MainMenuState>();
    }
    void Context::handle_event(Event event) { _state->handle_event(event); }
    State::State(game::state::Context& context) : _context(context) {}
    config::Config const& State::config() const { return _context._config; }

}  // namespace game::state
