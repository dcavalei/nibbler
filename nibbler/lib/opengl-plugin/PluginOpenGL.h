#pragma once

#include <string>

#include "game/IMenu.h"
#include "plugin/IPlugin.h"

namespace interface {
    class PluginOpenGL : public interface::IPlugin {
       public:
        std::string greet() override;
        void register_callback(Input input, signal sig) override;
        void setup_completed() override;
        void display_menu(IMenu const &) override;
    };
}  // namespace interface
