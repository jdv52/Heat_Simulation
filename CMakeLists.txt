cmake_minimum_required(VERSION 3.22)
project(heat_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SFML COMPONENTS graphics window system REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${Eigen3_INCLUDE_DIRS}  
)

add_library(heat_sim_lib STATIC 
    src/HeatMap.cpp 
    src/PDE.cpp
    src/ForwardDifference.cpp
    src/PDESolver.cpp
)

add_executable(heat_sim src/main.cpp)
target_link_libraries(heat_sim sfml-graphics sfml-window sfml-system Eigen3::Eigen heat_sim_lib)