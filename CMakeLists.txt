cmake_minimum_required(VERSION 3.20)

project(GameEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("Common")
add_subdirectory("Editor")
# add_subdirectory("GameRunner")
# add_subdirectory("Launcher")
