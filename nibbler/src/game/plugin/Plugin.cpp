#include "Plugin.h"

#include <dlfcn.h>
#include <spdlog/spdlog.h>

#include "exception.h"

namespace game::plugin {
    Plugin::~Plugin() { clear(); }

    interface::IPlugin& Plugin::instance() {
        if (_plugin)
            return *_plugin;
        else
            throw exception::PluginBadAccessException();
    }

    void Plugin::load() {
        if (_plugin) {
            spdlog::warn("Plugin({}) previously loaded...", id);
            return;
        }

        if (auto handle = dlopen(_path.c_str(), RTLD_LAZY | RTLD_LOCAL); handle) {
            auto deleter = [this](void* ptr) {
                spdlog::debug("[Handler Destructor (lambda)] Destroying handler of Plugin({})", id);
                dlclose(ptr);
            };
            _handle = std::unique_ptr<void, decltype(deleter)>(handle, std::move(deleter));
            spdlog::info("Handler of Plugin({}) successfully created!", id);
        } else {
            spdlog::critical("Handler of Plugin({}) failed to load with Path({})!", id, _path);
            throw exception::PluginHandleException();
        }

        auto create = reinterpret_cast<decltype(&interface::create)>(dlsym(_handle.get(), "create"));
        if (auto e = dlerror()) {
            spdlog::critical("Cannot load symbol 'create': {}", e);
            clear();
            throw exception::PluginInstanceException();
        }
        auto destroy = reinterpret_cast<decltype(&interface::destroy)>(dlsym(_handle.get(), "destroy"));
        if (auto e = dlerror()) {
            spdlog::critical("Cannot load symbol 'destroy': {}", e);
            clear();
            throw exception::PluginInstanceException();
        }

        if (auto plugin = create(); plugin) {
            auto deleter = [this, destroy](interface::IPlugin* ptr) {
                spdlog::debug("[IPlugin Destructor (lambda)] Destroying Plugin({})", id);
                destroy(ptr);
            };
            _plugin = std::unique_ptr<interface::IPlugin, decltype(deleter)>(plugin, std::move(deleter));
            spdlog::info("Plugin({}) instance successfully created!", id);
        } else {
            spdlog::critical("Failed to create Plugin({}) instance!", id);
            clear();
            throw exception::PluginInstanceException();
        }
        spdlog::info("Plugin({}) successfully loaded!", id);
    }

    void Plugin::clear() {
        // Order of destruction matters, _plugin must be destroyed before _handle.
        spdlog::debug("[clear] Plugin({}) being deleted...", id);
        _plugin.reset();
        _handle.reset();
    }
    Plugin::Plugin(std::string id, std::string path) : id(std::move(id)), _path(std::move(path)) {}

}  // namespace game::plugin