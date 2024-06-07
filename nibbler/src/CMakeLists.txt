project(nibbler)

add_executable(${PROJECT_NAME} main/main.cpp
        game/plugin/PluginManager.cpp
        game/plugin/PluginManager.h
        game/logic/StateMachine.cpp
        game/logic/StateMachine.h
        game/menu/menu.h
        game/App.h
        game/App.cpp
        game/plugin/exception.h
        game/plugin/Plugin.h
        game/plugin/Plugin.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)
target_include_directories(${PROJECT_NAME} PUBLIC ../include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
