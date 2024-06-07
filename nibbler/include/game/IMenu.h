#pragma once

#include <optional>
#include <string_view>
#include <vector>

namespace interface {
    struct IMenu {
        std::string_view name;
        std::vector<std::string_view> options;
        unsigned short hover_pos;
    };
}  // namespace interface
