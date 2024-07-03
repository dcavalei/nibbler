#include "MainMenuState.h"

#include "game/state/impl/PlayingState.h"

namespace game::state::impl {
    MainMenuState::MainMenuState(Context& context)
        : State(context), _menu({.name = "Main Menu", .hover_pos = 0, .options = {PLAY, EXIT}}) {}

    void MainMenuState::handle_event(Event event) {
        switch (event) {
            case Event::EXIT: _exit(); break;
            case Event::UP: _change_hover_position(true); break;
            case Event::DOWN: _change_hover_position(false); break;
            case Event::ENTER: _enter(); break;
            default: SPDLOG_DEBUG("Ignoring event {}", toString(event));
        }
    }
    void MainMenuState::_change_hover_position(bool up) {
        if (up && _menu.hover_pos == 0) {
            _menu.hover_pos = _menu.options.size() - 1;
        } else if (up) {
            _menu.hover_pos = _menu.hover_pos - 1;
        } else {
            _menu.hover_pos = (_menu.hover_pos + 1) % _menu.options.size();
        }
        SPDLOG_DEBUG("Option({}) selected", _menu.options.at(_menu.hover_pos));
    }
    void MainMenuState::_exit() { context_change_state<ExitState>(); }
    void MainMenuState::_enter() {
        if (_menu.options.at(_menu.hover_pos) == PLAY) {
            context_change_state<impl::PlayingState>();

        } else if (_menu.options.at(_menu.hover_pos) == EXIT) {
            _exit();
        }
    }
}  // namespace game::state::impl
