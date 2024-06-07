add_subdirectory(3rd-party)

SET(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
add_subdirectory(lib)
add_subdirectory(src)
