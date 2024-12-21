cmake_minimum_required(VERSION 3.22)
project(heat_sim LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(SFML COMPONENTS graphics window system REQUIRED)

add_library(heat_sim_lib STATIC src/HeatMap.cpp)

add_executable(heat_sim src/main.cpp)
target_link_libraries(heat_sim sfml-graphics sfml-window sfml-system heat_sim_lib)