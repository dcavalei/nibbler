#pragma once

#include <optional>
#include <string_view>
#include <vector>

namespace interface {
    /** @brief Interface that represents a menu to be displayed by the shared object (plugin). */
    struct Menu {
        /** @brief Menu name */
        std::string_view name;
        /** @brief Current hover option in the menu */
        unsigned short hover_pos;
        /** @brief List of available options */
        std::vector<std::string_view> options;
    };
}  // namespace interface
