#pragma once

#include <functional>
#include <string>

#include "game/IMenu.h"

// The reason why I'm doing an interface namespace is to integrate with
// IDE's... Clion can "link" the symbols which is nice for the plugin
// development ^^
namespace interface {
    using signal = std::function<void()>;

    // enum class representing keys from the keyboard
    enum class Input : uint8_t {
        A,
        S,
        D,
        W,
        ARROW_UP,
        ARROW_DOWN,
        ARROW_LEFT,
        ARROW_RIGHT,
        ENTER,
        ESC,
    };

    // Interface to be implemented by the SO (plugin).
    struct IPlugin {
        // Can be overridden by the plugin to execute cleanup code, implementation
        // dependent. Creating a VTable for the Destructor ensures that the correct
        // destructor is called.
        virtual ~IPlugin() = default;
        virtual std::string greet() = 0;
        /* Setup Functions - START */
        // To register Inputs from the player. The main application will provide a
        // signal (callable) that MUST be called by the plugin when <Input> is
        // pressed.
        virtual void register_callback(Input, signal) = 0;
        // To be called by the main application when the setup is completed.
        // The idea is to inform the plugin that no further setup is required.
        virtual void setup_completed() = 0;
        /* Setup Functions - END */

        // Menu instance with information of the menu that must be displayed
        virtual void display_menu(IMenu const &) = 0;
        //  virtual void display_game(...) = 0; TODO
    };

    // Using extern "C" with namespace is fine, it creates a symbol with the
    // function name.
    extern "C" {
    IPlugin *create(void);
    void destroy(IPlugin *);
    }

    static std::string_view toString(Input input) {
        switch (input) {
            case Input::ARROW_UP: return "ARROW_UP";
            case Input::ARROW_DOWN: return "ARROW_DOWN";
            case Input::ARROW_LEFT: return "ARROW_LEFT";
            case Input::ARROW_RIGHT: return "ARROW_RIGHT";
            case Input::ENTER: return "ENTER";
            case Input::ESC: return "ESC";
            default: return "WTF";
        }
    }
}  // namespace interface
