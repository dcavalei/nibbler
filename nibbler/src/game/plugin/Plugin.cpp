#include "Plugin.h"

#include <dlfcn.h>
#include <spdlog/spdlog.h>

#include "exception.h"

namespace game::plugin {
    Plugin::Plugin(std::string id, std::string path) : id(std::move(id)), _path(std::move(path)) {}

    Plugin::~Plugin() { clear(); }

    interface::IPlugin& Plugin::instance() {
        if (_plugin)
            return *_plugin;
        else
            throw exception::PluginBadAccessException();
    }

    void Plugin::load() {
        if (_plugin) {
            SPDLOG_WARN("Plugin({}) previously loaded...", id);
            return;
        }

        if (auto handle = dlopen(_path.c_str(), RTLD_LAZY | RTLD_LOCAL); handle) {
            auto deleter = [this](void* ptr) {
                if (!ptr) return;
                SPDLOG_DEBUG("Destroying handler of Plugin({})", id);
                dlclose(ptr);
            };
            _handle = std::unique_ptr<void, decltype(deleter)>(handle, std::move(deleter));
            SPDLOG_INFO("Handler of Plugin({}) successfully created!", id);
        } else {
            SPDLOG_CRITICAL("Handler of Plugin({}) failed to load with Path({})!", id, _path);
            throw exception::PluginHandleException();
        }

        auto create = reinterpret_cast<decltype(&interface::create)>(dlsym(_handle.get(), "create"));
        if (auto e = dlerror()) {
            SPDLOG_CRITICAL("Cannot load symbol 'create': {}", e);
            clear();
            throw exception::PluginInstanceException();
        }
        auto destroy = reinterpret_cast<decltype(&interface::destroy)>(dlsym(_handle.get(), "destroy"));
        if (auto e = dlerror()) {
            SPDLOG_CRITICAL("Cannot load symbol 'destroy': {}", e);
            clear();
            throw exception::PluginInstanceException();
        }

        if (auto plugin = create(); plugin) {
            auto deleter = [this, destroy](interface::IPlugin* ptr) {
                if (!ptr) return;
                SPDLOG_DEBUG("Destroying Plugin({})", id);
                destroy(ptr);
            };
            _plugin = std::unique_ptr<interface::IPlugin, decltype(deleter)>(plugin, std::move(deleter));
            SPDLOG_INFO("Plugin({}) instance successfully created!", id);
        } else {
            SPDLOG_CRITICAL("Failed to create Plugin({}) instance!", id);
            clear();
            throw exception::PluginInstanceException();
        }
        SPDLOG_INFO("Plugin({}) successfully loaded!", id);
    }

    void Plugin::clear() {
        _plugin.reset();
        _handle.reset();
    }

    void PluginManager::switch_plugin(std::string const& new_id) {
        // Unload plugin
        if (_current != _plugins.end()) {
            _current->second.clear();
            _current = _plugins.end();
        }

        if (auto it = _plugins.find(new_id); it != _plugins.end()) {
            _current = it;
            _current->second.load();
            SPDLOG_INFO("PluginManager successfully switch to Plugin({})", new_id);
        } else {
            SPDLOG_CRITICAL("PluginManager does not manage Plugin({})!!!", new_id);
            throw exception::PluginManagerBadAccessException();
        }
    }

    PluginManager& PluginManager::add(Plugin&& plugin) {
        std::string id(plugin.id);
        if (auto [it, ret] = _plugins.insert({id, std::move(plugin)}); ret) {
            SPDLOG_DEBUG("Succeeded to add Plugin({}) to PluginManager", it->first);
        } else {
            SPDLOG_WARN("Failed to add Plugin({}) to PluginManager!", id);
        }
        return *this;
    }

    interface::IPlugin& PluginManager::instance() {
        if (_current != _plugins.end())
            return _current->second.instance();
        else
            throw exception::PluginManagerNoPluginException();
    }

    PluginSwitcher::PluginSwitcher(std::span<std::string_view> list) {
        SPDLOG_INFO("PluginSwitcher starting...");
        // this code is kinda meh...
        uint i = 0;
        for (auto& item : list) {
            _plugin_manager.add({std::to_string(++i), item.data()});
        }
    }

    void PluginSwitcher::register_setup(std::function<void(interface::IPlugin&)>&& setup_func) {
        SPDLOG_DEBUG("Registered setup function");
        _setup_func = setup_func;
    }

    void PluginSwitcher::switch_plugin(std::string const& index) {
        try {
            _plugin_manager.instance().request_shutdown().wait();  // TODO: wait_for() ???
        } catch (exception::PluginManagerNoPluginException& error) {
            static bool blow_this_shit = false;
            if (blow_this_shit)
                throw error;
            else
                blow_this_shit = true;
            SPDLOG_WARN("This is shit coded, expected to throw once, TODO FIX THIS CRAP");
        }

        _plugin_manager.switch_plugin(index);
        _setup_func(_plugin_manager.instance());
    }
    PluginSwitcher::~PluginSwitcher() {
        try {
            auto status = _plugin_manager.instance().request_shutdown().wait_for(std::chrono::seconds(1));
            switch (status) {
                case std::future_status::timeout: SPDLOG_CRITICAL("The application might kaput, oh boy!!"); break;
                default: SPDLOG_DEBUG("No kaput, we gucci");
            }
        } catch (exception::PluginManagerNoPluginException& error) {
            SPDLOG_WARN("{}", error.what());
        }
    }
}  // namespace game::plugin