#pragma once

#include <memory>
#include <string>
#include <thread>

#include "game/Menu.h"
#include "plugin/IPlugin.h"

namespace interface {
    class PluginOpenGL : public interface::IPlugin {
       public:
        std::string greet() override;
        void register_cb(Input input, Callback sig) override;
        void setup_completed(std::future<void> future) override;
        std::future<void> request_shutdown() override;
        void display_menu(Menu const &) override;
        void display_game(std::vector<ARGB> const &) override;
    };
}  // namespace interface
