cmake_minimum_required(VERSION 3.16)

project(tsp C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

set(TSP_SOURCES
    include/distance.c
    include/result.c
    include/thread.c
    include/utils.c
)

add_executable(tsp main.c ${TSP_SOURCES})
target_compile_options(tsp PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(tsp PRIVATE Threads::Threads m)

add_executable(tsp_test tests/test_tsp.c ${TSP_SOURCES})
target_compile_definitions(tsp_test PRIVATE N=5)
target_compile_options(tsp_test PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(tsp_test PRIVATE Threads::Threads m)

enable_testing()
add_test(NAME tsp_test COMMAND tsp_test)
