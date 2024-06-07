#include "PluginManager.h"

namespace game::plugin {
    void PluginManager::switch_plugin(std::string const& new_id) {
        // Unload plugin
        if (_current != _plugins.end()) {
            _current->second.clear();
            _current = _plugins.end();
        }

        if (auto it = _plugins.find(new_id); it != _plugins.end()) {
            _current = it;
            _current->second.load();
            spdlog::info("PluginManager successfully switch to Plugin({})", new_id);
        } else {
            spdlog::critical("PluginManager does not manage Plugin({})!!!", new_id);
            throw exception::PluginManagerBadAccessException();
        }
    }
    PluginManager& PluginManager::add(Plugin&& plugin) {
        std::string id(plugin.id);
        if (auto [it, ret] = _plugins.insert({id, std::move(plugin)}); ret) {
            spdlog::debug("Succeeded to add Plugin({}) to PluginManager", it->first);
        } else {
            spdlog::warn("Failed to add Plugin({}) to PluginManager!", id);
        }
        return *this;
    }

    interface::IPlugin& PluginManager::instance() {
        if (_current != _plugins.end())
            return _current->second.instance();
        else
            throw exception::PluginManagerNoPluginException();
    }
}  // namespace game::plugin
