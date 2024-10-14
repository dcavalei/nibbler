#if SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_TRACE
// https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
#define PATTERN "[%^%l%$]\t[%t] [%!] %v"
#else
#define PATTERN "[%^%l%$] %v"
#endif

#include "App.h"

#include "game/snake/Snake.h"
#include "game/state/impl/ExitState.h"
#include "literals.h"

namespace game {
    App::App(int ac, char const *av[]) : _args(av, av + ac) {
        spdlog::set_level(spdlog::level::trace);
        spdlog::flush_on(spdlog::level::trace);
        spdlog::set_pattern(PATTERN);
        SPDLOG_DEBUG("Args: {}", fmt::join(_args, " ."));
    }

    void App::init() {
        _worker = std::make_unique<worker::Worker>(2);
        _event_queue = std::make_unique<state::EventQueue>();
        _plugin_switcher = std::make_unique<plugin::PluginSwitcher>(std::span{++_args.begin(), _args.end()});
        _conf = std::make_unique<config::Config>();

        // TODO improve
        _conf->game_height = 20;
        _conf->game_width = 20;
        _context = std::make_unique<state::Context>(*_conf);
        _context->subscribe<state::impl::ExitState>([this] {
            auto &[exit, cv, _] = this->_lmao_exit;
            exit = true;
            cv.notify_one();
        });

        // TODO: Use an Adapter design pattern to convert from Event to Domain specific Event...
        _event_queue->subscribe([this](state::Event event) { _plugin_switcher->handle_event(event); });
        _event_queue->subscribe([this](state::Event event) { _context->handle_event(event); });

        _plugin_switcher->register_setup([this](interface::IPlugin &plugin) {
            SPDLOG_DEBUG("Setting up Plugin...");
            // make this better, this is smelly asf
            std::promise<void> promise;
            std::thread([&plugin, future = promise.get_future()]() mutable {
                plugin.entrypoint(std::move(future));
            }).detach();

            using interface::Input;
            plugin.register_cb(Input::ENTER, [this] { _event_queue->push(state::Event::ENTER); });
            plugin.register_cb(Input::ESC, [this] { _event_queue->push(state::Event::EXIT); });
            plugin.register_cb(Input::A, [this] { _event_queue->push(state::Event::LEFT); });
            plugin.register_cb(Input::S, [this] { _event_queue->push(state::Event::DOWN); });
            plugin.register_cb(Input::D, [this] { _event_queue->push(state::Event::RIGHT); });
            plugin.register_cb(Input::W, [this] { _event_queue->push(state::Event::UP); });
            plugin.register_cb(Input::ONE, [this] { _event_queue->push(state::Event::PLUGIN_1); });
            plugin.register_cb(Input::TWO, [this] { _event_queue->push(state::Event::PLUGIN_1); });
            plugin.register_cb(Input::THREE, [this] { _event_queue->push(state::Event::PLUGIN_3); });
            promise.set_value();
        });
        _plugin_switcher->switch_plugin("1");

        auto snake = snake::Snake(20, 20);
    }

    void App::run() {
        auto &[exit, cv, mtx] = _lmao_exit;
        std::unique_lock lk(mtx);
        cv.wait(lk, [&exit]() { return bool(exit); });
    }
}  // namespace game
