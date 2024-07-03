#pragma once

#include <functional>
#include <future>
#include <string>

#include "game/Menu.h"

// The reason why I'm doing an interface namespace is to integrate with
// IDE's... Clion can "link" the symbols which is nice for the plugin
// development ^^
namespace interface {
    using Callback = std::function<void()>;

    using ARGB = uint32_t;

    /**
     * @brief Enum class representing keys from the keyboard.
     *
     * This enum class defines various keyboard keys that can be used as inputs
     * for registering callbacks.
     */
    enum class Input : uint8_t {
        A,            ///< Key 'A'
        S,            ///< Key 'S'
        D,            ///< Key 'D'
        W,            ///< Key 'W'
        ONE,          ///< Key '1'
        TWO,          ///< Key '2'
        THREE,        ///< Key '3'
        ARROW_UP,     ///< Up arrow key
        ARROW_DOWN,   ///< Down arrow key
        ARROW_LEFT,   ///< Left arrow key
        ARROW_RIGHT,  ///< Right arrow key
        ENTER,        ///< Enter key
        ESC           ///< Escape key
    };

    [[maybe_unused]] constexpr std::string_view toString(Input input) {
        switch (input) {
            case Input::ARROW_UP: return "ARROW_UP";
            case Input::ARROW_DOWN: return "ARROW_DOWN";
            case Input::ARROW_LEFT: return "ARROW_LEFT";
            case Input::ARROW_RIGHT: return "ARROW_RIGHT";
            case Input::ENTER: return "ENTER";
            case Input::ESC: return "ESC";
            case Input::A: return "A";
            case Input::S: return "S";
            case Input::D: return "D";
            case Input::W: return "W";
            case Input::ONE: return "ONE";
            case Input::TWO: return "TWO";
            case Input::THREE: return "THREE";
            default: return "WTF";
        }
    }

    // Interface to be implemented by the shared object (plugin).
    struct IPlugin {
        virtual ~IPlugin() = default;

        /**
         * @brief Returns a greeting message.
         *
         * This function returns a greeting message from the plugin.
         *
         * @returns A string containing the greeting message.
         */
        virtual std::string greet() = 0;

        /**
         * @brief Registers a callback for a specific input.
         *
         * This function will be called by the main application to register a callback
         * that will be invoked when the specified input is activated.
         *
         * @param input The input to be registered.
         * @param callback The callback function to be called when the input is activated.
         */
        virtual void register_cb(Input input, Callback callback) = 0;

        /**
         * @brief Informs the plugin that the main program setup is complete.
         *
         * This function will be called by the main application to notify the plugin
         * that the main program setup is finished. The function will be executed in a
         * dedicated thread.
         *
         * @param future A future that will be fulfilled by the main application when
         *               the setup is completed.
         */
        virtual void setup_completed(std::future<void> future) = 0;

        /**
         * @brief Requests the plugin to shut down.
         *
         * This function will be called by the main application to request the plugin
         * to perform any necessary cleanup and shut down.
         */
        [[nodiscard]] virtual std::future<void> request_shutdown() = 0;

        /**
         * @brief Displays the menu.
         *
         * This function will be called to display the specified menu within the plugin.
         *
         * @param menu A reference to the menu to be displayed.
         */
        virtual void display_menu(Menu const &menu) = 0;

        /**
         * @brief TODO
         * @param buffer
         */
        virtual void display_game(std::vector<ARGB> const &buffer) = 0;
    };

    // Using extern "C" with namespace is fine, it creates a symbol with the function name.
    extern "C" {
    /**
     * @brief Creates a new instance of IPlugin.
     *
     * This function allocates and initializes a new IPlugin instance on the heap.
     * The caller takes ownership of the returned pointer and is responsible for
     * managing its lifetime, including deallocating the instance by calling destroy().
     *
     * @returns A pointer to the newly created IPlugin instance.
     */
    IPlugin *create(void);

    /**
     * @brief Destroys an IPlugin instance.
     *
     * This function deallocates an IPlugin instance that was previously created
     * using create(). After calling this function, the pointer to the IPlugin
     * instance MUST NOT be used.
     *
     * @param plugin A pointer to the IPlugin instance to be destroyed.
     */
    void destroy(IPlugin *plugin);
    }

}  // namespace interface
