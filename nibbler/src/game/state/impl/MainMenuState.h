#pragma once

#include "game/Menu.h"
#include "game/plugin/Plugin.h"
#include "game/state/Context.h"
#include "game/state/impl/ExitState.h"

namespace game::state::impl {
    class MainMenuState final : public State {
       public:
        static constexpr std::string_view PLAY = "Play";
        static constexpr std::string_view EXIT = "Exit";

        explicit MainMenuState(Context& context);
        ~MainMenuState() override = default;

        void handle_event(Event event) override;

       private:
        void _change_hover_position(bool up);
        void _exit();
        void _enter();

       private:
        interface::Menu _menu;
    };

}  // namespace game::state::impl
