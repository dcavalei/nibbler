#pragma once

#include "game/plugin/Plugin.h"
#include "game/state/Context.h"
#include "game/state/Event.h"
#include "game/worker/Worker.h"

namespace game {
    class App {
       public:
        App() = delete;
        App(App &&) = delete;
        App(App const &) = delete;
        App(int ac, char const *av[]);

        void init();
        void run();

       private:
        std::vector<std::string_view> _args{};
        std::tuple<std::atomic<bool>, std::condition_variable, std::mutex> _lmao_exit;
        std::unique_ptr<plugin::PluginSwitcher> _plugin_switcher;
        std::unique_ptr<state::EventQueue> _event_queue;
        std::unique_ptr<state::Context> _context;
        std::unique_ptr<worker::Worker> _worker;
        std::unique_ptr<config::Config> _conf;
    };
}  // namespace game
