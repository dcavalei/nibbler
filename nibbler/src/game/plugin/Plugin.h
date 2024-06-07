#pragma once

#include <functional>
#include <memory>

#include "plugin/IPlugin.h"

namespace game::plugin {
    class Plugin {
       public:
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

}  // namespace game::plugin
