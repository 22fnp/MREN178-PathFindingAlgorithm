cmake_minimum_required(VERSION 3.26)
project(Group15PathFinder C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(Group15PathFinder
    src/main.c
    src/data_structures.c
    src/pathfinding.c
    src/simulation.c
)

add_executable(test_pathfinding
    tests/test_pathfinding.c
    src/data_structures.c
    src/pathfinding.c
    src/simulation.c
)

if (NOT MSVC)
    target_link_libraries(Group15PathFinder PRIVATE m)
    target_link_libraries(test_pathfinding PRIVATE m)
endif()

enable_testing()
add_test(NAME test_pathfinding COMMAND test_pathfinding)
