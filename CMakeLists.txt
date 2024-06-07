cmake_minimum_required(VERSION 3.24)
project(nibbler)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

if (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug")
    option(UBSAN_ENABLED "UB Sanitizer enabled" ON)
else ()
    option(CMAKE_BUILD_TYPE "Build type" Release)
endif ()

if (UBSAN_ENABLED AND (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU"))
    set(UBSAN_FLAGS "-fsanitize=undefined -fno-sanitize-recover=all")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${UBSAN_FLAGS}")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ${UBSAN_FLAGS}")
endif ()

add_subdirectory(nibbler)
