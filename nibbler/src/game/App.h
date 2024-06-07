#pragma once

#include "game/logic/StateMachine.h"
#include "game/plugin/PluginManager.h"

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
        plugin::PluginManager _pm;
        logic::StateMachine _sm;
    };
}  // namespace game
