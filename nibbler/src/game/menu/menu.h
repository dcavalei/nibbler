#pragma once

#include <array>

#include "game/IMenu.h"

namespace game::menu {

    struct MainMenu : public interface::IMenu {
        static constexpr std::string_view NAME{"Main Menu"};
        static constexpr std::array<std::string_view, 2> OPTIONS{"Play", "Exit"};
        MainMenu() : IMenu({.name = NAME, .options = {OPTIONS.begin(), OPTIONS.end()}, .hover_pos = 0}) {}
    };
}  // namespace game::menu
