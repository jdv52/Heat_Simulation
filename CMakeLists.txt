cmake_minimum_required(VERSION 3.22)
project(heat_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SFML_STATIC_LIBRARIES ON)

add_subdirectory(dependencies)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

add_library(model 
  src/model/Mesh.cpp
  src/model/GridRepresentation.cpp
  src/model/ConvectionDiffusionPDE.cpp
  src/model/AbstractPDE.cpp
)

target_link_libraries(model PUBLIC Eigen3::Eigen)

add_executable(heat_sim 
  src/main.cpp 
  src/HeatMap.cpp 
  src/SimulationManager.cpp 
  src/SimulationWindow.cpp 
  src/SimulationToolbar.cpp 
  src/SimulationControlPanel.cpp
  src/SimulationCommands.cpp
  src/HeatMapGradient.cpp
  src/Simulation.cpp
  src/ConfigManager.cpp
)
target_link_libraries(heat_sim
  PRIVATE
  model
  sfml-graphics sfml-system sfml-window
  ImGui-SFML::ImGui-SFML
  argparse
  inicpp::inicpp
)
target_compile_options(heat_sim PRIVATE -Wall -Wextra -Werror -Wconversion)

add_library(forward_difference SHARED src/model/ForwardDifferenceSolver.cpp)

set_target_properties(forward_difference PROPERTIES
  CXX_VISIBILITY_PRESET hidden
  LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/plugins"
)

target_link_libraries(forward_difference PRIVATE model)
