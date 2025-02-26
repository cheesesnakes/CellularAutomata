cmake_minimum_required(VERSION 3.10)

# set the project name

project(CellulartAutomata)

# set the C++ standard

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add SFML

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# add the executable

add_executable(CellulartAutomata main.cpp)

# link SFML

target_link_libraries(CellulartAutomata sfml-graphics sfml-window sfml-system)