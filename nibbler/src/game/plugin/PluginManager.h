#pragma once

#include <dlfcn.h>
#include <spdlog/spdlog.h>

#include <string>

#include "game/plugin/Plugin.h"
#include "game/plugin/exception.h"
#include "plugin/IPlugin.h"

namespace game::plugin {
    class PluginManager {
       public:
        PluginManager() = default;
        PluginManager(PluginManager const &) = delete;
        PluginManager(PluginManager &&) = delete;
        PluginManager &add(Plugin &&plugin);
        void switch_plugin(std::string const &new_id);
        interface::IPlugin &instance();

       private:
        std::unordered_map<std::string, Plugin> _plugins;
        std::unordered_map<std::string, Plugin>::iterator _current;
    };
}  // namespace game::plugin
