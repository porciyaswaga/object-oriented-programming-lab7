cmake_minimum_required(VERSION 3.14)
project(npc_game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
include_directories(include)

set(SOURCES src/NPC.cpp src/NPCFactory.cpp src/Observer.cpp src/Game.cpp)

add_executable(npc_game main.cpp ${SOURCES})
target_link_libraries(npc_game Threads::Threads)

include(FetchContent)
FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG release-1.12.1)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests tests/test_npc.cpp ${SOURCES})
target_link_libraries(tests GTest::gtest_main Threads::Threads)
include(GoogleTest)
gtest_discover_tests(tests)
