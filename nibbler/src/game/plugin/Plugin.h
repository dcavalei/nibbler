#pragma once

#include <functional>
#include <memory>
#include <span>

#include "game/state/Event.h"
#include "plugin/IPlugin.h"

namespace game::plugin {

    /**
     * @brief The Plugin class responsible for handling the dlopen() api.
     *
     * Manages the handle ptr, ensuring correct construction and destruction. It's not copyable, only movable.
     * Owns the Plugin instance, also responsible for it's destruction. The expected usage is:
     *
     * auto p = Plugin("some_unique_id", "/path/to/shared/object");
     * p.load();
     * p.instance()->whatever_methods();
     */
    class Plugin {
        template <typename T>
        using unique_ptr = std::unique_ptr<T, std::function<void(T *)> >;

       public:
        Plugin() = delete;
        Plugin(Plugin const &) = delete;
        Plugin(Plugin &&) = default;
        /**
         * @brief Plugin constructor.
         *
         * @param id The ID name of the plugin.
         * @param path The Path to the shared object (.so).
         */
        Plugin(std::string id, std::string path);
        /** @brief Plugin destructor, will destroy the handle/plugin, if any */
        ~Plugin();

        /**
         * @brief Destroy the handle/plugin instance, if any
         *
         * This is needed so that we can switch plugins.
         */
        void clear();
        /**
         * @brief Plugin instance method.
         *
         * @return A reference to the currently loaded plugin instance, or throw if there's no instance.
         */
        interface::IPlugin &instance();

        /**
         * @brief Load the instance associated with shared object path.
         *
         * This will make the plugin instance available for usage.
         * This method must always be called before using the instance() method.
         */
        void load();

       public:
        /** @brief Unique Identifier */
        const std::string id;

       private:
        /** @brief Path to the shared object */
        const std::string _path;
        /** @brief Pointer to the plugin instance */
        unique_ptr<interface::IPlugin> _plugin;
        /** @brief Pointer to the dlopen() api handle */
        unique_ptr<void> _handle;
    };

    /**
     * @brief The PluginManager class, responsible for managing multiple plugins.
     *
     * This class guarantees that only one plugin is loaded at the time, and that the Plugin api is used as expected.
     * when switching Plugin, the previously loaded Plugin is destroyed and a new one is loaded. It also allows you to
     * use the current instance with the instance() method, similar to Plugin::instance().
     */
    class PluginManager {
        using iterator = std::unordered_map<std::string, Plugin>::iterator;

       public:
        PluginManager(PluginManager const &) = delete;
        PluginManager(PluginManager &&) = delete;

        PluginManager() = default;

        /**
         * @brief Add a new Plugin by taking ownership of an existing one.
         *
         * @param plugin The Plugin to be moved.
         * @return A Reference to Self.
         */
        PluginManager &add(Plugin &&plugin);
        /**
         * @brief Switch to another Plugin
         *
         * This method will clear() the old Plugin and load() a new Plugin().
         *
         * @param new_id The UID of the Plugin to be switched to.
         */
        void switch_plugin(std::string const &new_id);

        /**
         * @brief The currently loaded Plugin instance.
         *
         * @return A reference to the currently loaded plugin instance.
         */
        interface::IPlugin &instance();

       private:
        /** @brief The iterator to the currently loaded instance, if any */
        iterator _current{};
        /** @brief The map that holds the plugins, where the key is the Plugin UID */
        std::unordered_map<std::string, Plugin> _plugins{};
    };

    /**
     * @brief The Plugin instance class
     *
     * Responsible registering a function that will be called after switching the plugin.
     *
     * PluginSwitcher asks the PluginManager to switch Plugin, before switching it will request the Plugin instance to
     * gracefully shutdown. After switching it will call the registered function.
     *
     * it will also expose a way to handle events for the event handler.
     */
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
        explicit PluginSwitcher(std::span<std::string_view> list);

        /**
         * @brief The PluginSwitcher destructor
         *
         * Responsible for requesting the currently loaded instance to shutdown gracefully, and wait.
         */
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

        /** @brief Method to be called by the event handler class */
        void handle_event(state::Event event);

       private:
        PluginManager _plugin_manager;
        std::function<void(interface::IPlugin &)> _setup_func;
    };
}  // namespace game::plugin
