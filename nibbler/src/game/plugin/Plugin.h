#pragma once

#include <functional>
#include <memory>
#include <span>

#include "game/state/Event.h"
#include "plugin/IPlugin.h"

namespace game::plugin {
    class Plugin {
        template <typename T>
        using unique_ptr = std::unique_ptr<T, std::function<void(T *)> >;

       public:
        Plugin() = delete;
        Plugin(Plugin const &) = delete;

        Plugin(Plugin &&) = default;
        Plugin(std::string id, std::string path);
        ~Plugin();

        void clear();
        interface::IPlugin &instance();
        void load();

       public:
        const std::string id;

       private:
        const std::string _path;
        unique_ptr<interface::IPlugin> _plugin;
        unique_ptr<void> _handle;
    };

    class PluginManager {
        using iterator = std::unordered_map<std::string, Plugin>::iterator;

       public:
        PluginManager(PluginManager const &) = delete;
        PluginManager(PluginManager &&) = delete;

        PluginManager() = default;

        PluginManager &add(Plugin &&plugin);
        void switch_plugin(std::string const &new_id);
        interface::IPlugin &instance();

       private:
        iterator _current{};
        std::unordered_map<std::string, Plugin> _plugins{};
    };

    class PluginSwitcher {
       public:
        /**
         * @brief PluginSwitcher Constructor
         *
         * Constructs a PluginSwitcher object that initializes the plugin manager
         * with the provided list of paths to plugin shared objects (*.so).
         * Each path in the initializer list is associated with a string representing the index.
         *
         * @param list An initializer_list of strings representing the paths to load plugins from.
         */
        PluginSwitcher(std::span<std::string_view> list);

        ~PluginSwitcher();

        /**
         * @brief Register setup function to be called each time the plugin is switched.
         * @param setup_func Function to be called.
         */
        void register_setup(std::function<void(interface::IPlugin &)> &&setup_func);

        /**
         * @brief Switch the current loaded plugin and call the registered setup function.
         * @param index Identifier
         */
        void switch_plugin(std::string const &index);

        void handle_event(state::Event event) {
            switch (event) {
                case state::Event::PLUGIN_1: switch_plugin("1"); break;
                case state::Event::PLUGIN_2: switch_plugin("2"); break;
                case state::Event::PLUGIN_3: switch_plugin("3"); break;
                default: SPDLOG_DEBUG("Ignoring event {}", toString(event));
            }
        }

       private:
        PluginManager _plugin_manager;
        std::function<void(interface::IPlugin &)> _setup_func;
    };
}  // namespace game::plugin
