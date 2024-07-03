project(nibbler)

add_executable(${PROJECT_NAME} main/main.cpp
        game/App.h
        game/App.cpp
        game/plugin/exception.h
        game/plugin/Plugin.h
        game/plugin/Plugin.cpp
        game/state/Context.h
        game/state/Context.cpp
        game/state/Event.h
        game/state/Event.cpp
        game/state/impl/MainMenuState.h
        game/state/impl/ExitState.h
        game/state/impl/MainMenuState.cpp
        game/state/impl/ExitState.cpp
        game/worker/Worker.cpp
        game/worker/Worker.h
        game/snake/Snake.cpp
        game/snake/Snake.h
        game/snake/exception.h
        game/state/impl/PlayingState.cpp
        game/state/impl/PlayingState.h
        game/Config.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)
target_include_directories(${PROJECT_NAME} PUBLIC ../include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(${PROJECT_NAME} PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
