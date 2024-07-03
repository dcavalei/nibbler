#pragma once

#include <functional>
#include <future>
#include <string>

#include "game/Menu.h"

// The reason why I'm doing an interface namespace is to integrate with IDE's.
// Clion can index the symbols, which is nice for the plugin development.
namespace interface {
    /** @brief Type alias for a callback function. */
    using Callback = std::function<void()>;

    using ARGB = uint32_t;

    /** @brief Enum class representing keys from the keyboard. */
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

    /**
     * @brief Input to std::string mapper.
     * @param input Enum to map.
     * @returns std::string_view associated with the Input.
     */
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

    /** @brief Interface to be implemented by the shared object (plugin). */
    struct IPlugin {
        virtual ~IPlugin() = default;

        /**
         * @brief Returns a greeting message.
         *
         * This function returns a greeting message from the plugin. // TODO: Remove??
         *
         * @returns A string containing the greeting message.
         */
        virtual std::string greet() = 0;

        /**
         * @brief Registers a callback for a specific input (key).
         *
         * This function registers a Callback that MUST be called by the graphical library (plugin)
         * when the the given Input (key) is pressed. This is how the application receives events.
         *
         * @param input The Input to be pressed.
         * @param callback The Callback function to be called.
         */
        virtual void register_cb(Input input, Callback callback) = 0;

        /**
         * @brief Entrypoint for the graphical library (plugin).
         *
         * This function will be called by the application to initialize the plugin.
         *
         * From the application's perspective:
         * - The application is ready to give the plugin some control. It calls entrypoint() so that the application
         *   can start to register callbacks.
         * - When the application is done with the setup, i.e., when no further information needs to be passed to the
         *   plugin, it will fulfill the future.
         * - The application must take into consideration that the graphical library might implement its own loop,
         *   which might block the thread.
         * - This function MUST be executed asynchronously.
         *
         * From the graphical library's perspective (plugin):
         * - The application is requesting the plugin to initialize.
         * - The plugin can start to perform some setup if it does NOT depend on information from the application side.
         * - The plugin will wait for the application to fulfill the future passed as a parameter.
         * - When the future is fulfilled, the plugin has the guarantee that all the information has been provided, and
         *   it can finish the setup. If needed, the plugin can block the current thread.
         *
         * @param future A future that will be fulfilled by the application when the setup is completed.
         */
        virtual void entrypoint(std::future<void> future) = 0;

        /**
         * @brief Requests the graphical library (plugin) to shut down.
         *
         * This function will be called by the application to request the plugin to perform a graceful shutdown.
         *
         * From the application's perspective:
         * - The application intends to stop using the plugin and calls request_shutdown() to pass that intention to
         *   the plugin.
         * - The application expects that the plugin will not block the current thread. Instead of blocking, it expects
         *   to receive a future that will be fulfilled by the plugin, signaling that it is safe to destroy the plugin
         *   instance.
         * - The application expects the plugin to comply with RAII (Resource Acquisition Is Initialization) concepts,
         *   where the scope boundaries are entrypoint() and request_shutdown().
         * - NOTE: While this is not strictly RAII compliant, RAII is used here to emphasize the guarantees required by
         *   the application from the plugin.
         *
         * From the graphical library's perspective (plugin):
         * - The application is requesting the plugin to shut down.
         * - The plugin will NOT block the current thread and it will give back the control flow to the application by
         *   returning a future.
         * - The plugin ensures that all owned resources (i.e. threads, processes, heap allocations, etc..) are
         *   properly "freed".
         * - When the plugin sure is ready to be destroyed, it will fulfill the promise associated future, to signal
         *   the application that it is safe to do so.
         *
         * @returns std::future to be fulfilled by the plugin when the shutdown is finished.
         */
        [[nodiscard]] virtual std::future<void> request_shutdown() = 0;

        /**
         * @brief Display a generic Menu object.
         *
         * This function will be called by the application to request the plugin to render a Menu.
         *
         * @param menu The menu to be displayed.
         */
        virtual void display_menu(Menu const &menu) = 0;

        /**
         * @brief Display the current state of the Snake game.
         *
         * This function will be called by the application to request the plugin to draw the given buffer.
         *
         * Each pixel in the buffer is a uint32_t in ARGB format. The application is responsible for filling the
         * buffer, so the only responsibility of the plugin is to render the given buffer. The plugin does not know
         * about the game state and does not need to know.
         *
         * @param buffer The buffer containing the game's pixel data in ARGB format.
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
     * @returns IPlugin pointer to the newly created IPlugin instance.
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
