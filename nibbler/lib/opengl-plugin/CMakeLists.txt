project(opengl-plugin)

add_library(${PROJECT_NAME} SHARED
        PluginOpenGL.cpp
)

target_compile_definitions(${PROJECT_NAME} PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ../../include)

target_link_libraries(${PROJECT_NAME} spdlog GL glut GLU)
