#pragma once

#include <optional>
#include <string_view>
#include <vector>

namespace interface {
    struct Menu {
        std::string_view name;
        unsigned short hover_pos;
        std::vector<std::string_view> options;
    };
}  // namespace interface
