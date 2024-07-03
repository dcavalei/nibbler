#pragma once

#include <string>
#include <vector>
namespace game::config {
    struct Config {
        size_t game_width;
        size_t game_height;
        size_t game_speed_ms = 1000;
        std::vector<std::string> plugin_paths;
    };
}  // namespace game::config