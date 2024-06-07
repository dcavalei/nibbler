#pragma once

#include <exception>

namespace game::plugin::exception {
    struct PluginHandleException : public std::exception {
        [[nodiscard]] const char *what() const noexcept final { return "Failed to create a Plugin handle!"; }
    };

    struct PluginInstanceException : public std::exception {
        [[nodiscard]] const char *what() const noexcept final { return "Failed to create a Plugin instance!"; }
    };

    struct PluginBadAccessException : public std::exception {
        [[nodiscard]] const char *what() const noexcept final { return "Plugin not loaded!"; }
    };

    struct PluginManagerBadAccessException : public std::exception {
        [[nodiscard]] const char *what() const noexcept final { return "Plugin not managed by PluginManager!"; }
    };

    struct PluginManagerNoPluginException : public std::exception {
        [[nodiscard]] const char *what() const noexcept final { return "No instance loaded by PluginManager!"; }
    };
}  // namespace game::plugin::exception
