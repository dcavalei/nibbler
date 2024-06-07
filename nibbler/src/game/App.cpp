#include "App.h"

#include "game/logic/StateMachine.h"
#include "literals.h"

namespace game {
    App::App(int ac, char const *av[]) : _args(av, av + ac) {
        spdlog::set_level(spdlog::level::debug);
        spdlog::flush_on(spdlog::level::debug);
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
        spdlog::debug("App loaded with args: {}", fmt::join(_args, " "));
    }

    void App::init() {
        constexpr std::string_view DEBUG_PATH =
            "/home/dcavalei/Repos/nibbler/cmake-build-debug/nibbler/lib/"
            "opengl-plugin/libopengl-plugin.so";
        _pm.add(plugin::Plugin(literals::OPENGL_PLUGIN.data(), DEBUG_PATH.data()));
    }

    void App::run() {
        _pm.switch_plugin(literals::OPENGL_PLUGIN.data());
        auto &instance = _pm.instance();

        instance.register_callback(interface::Input::A, [this]() { this->_sm.queueEvent(logic::Event::LEFT); });
        instance.register_callback(interface::Input::S, [this]() { this->_sm.queueEvent(logic::Event::DOWN); });
        instance.register_callback(interface::Input::D, [this]() { this->_sm.queueEvent(logic::Event::RIGHT); });
        instance.register_callback(interface::Input::W, [this]() { this->_sm.queueEvent(logic::Event::UP); });
        instance.register_callback(interface::Input::ENTER, [this]() { this->_sm.queueEvent(logic::Event::ENTER); });
        instance.register_callback(interface::Input::ESC, [this]() { this->_sm.queueEvent(logic::Event::EXIT); });

        spdlog::info(instance.greet());
        instance.setup_completed();
        _sm.init();
        _sm.loop();
    }
}  // namespace game
