#pragma once

#include <typeindex>
#include <unordered_map>

#include "game/Config.h"
#include "game/plugin/Plugin.h"
#include "game/state/Event.h"
#include "spdlog/spdlog.h"

namespace game::state {
    class State;
    class Context {
        /**
         * @brief So that State can request Context to change the current state.
         */
        friend class State;

        using Callback = std::function<void()>;

       public:
        /**
         * @brief Context Constructor.
         */
        Context(config::Config& conf);
        ~Context() = default;

        /**
         * @brief Deploy event to current State.
         * @param event Event to be deployed.
         */
        void handle_event(Event event);

        /**
         * @brief Subscribes a callback to be called when the State changes to the specified T State.
         * @tparam T Concrete state to subscribe to. T must derive from State.
         * @param cb Callback to be called when the context changes to a state of type T.
         * @throws std::invalid_argument if T does not derive from State.
         */
        template <class T>
        void subscribe(Callback&& cb);

       private:
        /**
         * @brief Generic function responsible for State transitions.
         * This functions should only be used by State object, hence the friend class State.
         * @tparam T State to be changed to.
         */
        template <class T>
        void _change_state();

       private:
        config::Config& _config;
        std::unique_ptr<State> _state;
        std::unordered_map<std::string_view, std::vector<Callback>> subs;
    };

    class State {
       public:
        explicit State(Context& context);
        virtual ~State() = default;
        virtual void handle_event(Event event) = 0;

       protected:
        /**
         * @brief Friendship is not transitive (a friend of your friend is not your friend).
         * @tparam T State to be changed to.
         */
        template <class T>
        void context_change_state();

        config::Config const& config() const;

        Context& _context;
    };

    template <class T>
    void Context::subscribe(Callback&& cb) {
        static_assert(std::is_base_of_v<State, T>, "T must derive from State");
        SPDLOG_DEBUG("Callback 0x{:x} subscribed to State transition {}", (std::uintptr_t)&cb, typeid(T).name());
        subs[typeid(T).name()].push_back(cb);
    }

    template <class T>
    void Context::_change_state() {
        static_assert(std::is_base_of_v<State, T>, "T must derive from State");
        SPDLOG_DEBUG("Changing State({})", typeid(T).name());
        _state = std::make_unique<T>(*this);
        for (auto& cb : subs[typeid(T).name()]) {
            SPDLOG_DEBUG("Notifying Callback 0x{:x}", (std::uintptr_t)&cb);
            cb();
        }
    }

    template <class T>
    void State::context_change_state() {
        SPDLOG_DEBUG("Requesting Context to change State({})", typeid(T).name());
        _context._change_state<T>();
    }
}  // namespace game::state
