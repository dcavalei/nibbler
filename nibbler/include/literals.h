#pragma once

#include <string_view>

namespace literals {
    constexpr std::string_view OPENGL_PLUGIN = "opengl-plugin";
    /** @brief Initial player size */
    constexpr size_t PLAYER_INITIAL_SIZE = 4;
    /** @brief Board minimum width required to play the game */
    constexpr size_t BOARD_MINIMUM_WIDTH = 10;
    /** @brief Board minimum height required to play the game */
    constexpr size_t BOARD_MINIMUM_HEIGHT = 10;
}  // namespace literals
