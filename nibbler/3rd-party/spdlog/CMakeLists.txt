include(FetchContent)

# For some reason CMakeCache.txt shows OFF but it was built as shared object lol.
set(SPDLOG_BUILD_SHARED ON)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.14.1
)

FetchContent_MakeAvailable(spdlog)
