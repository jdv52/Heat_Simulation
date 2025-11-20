cmake_minimum_required(VERSION 3.22)
project(heat_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SFML_STATIC_LIBRARIES ON)

add_subdirectory(dependencies)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(heat_sim 
  src/main.cpp 
  src/PDESolver.cpp 
  src/PDE.cpp 
  src/HeatMapGradient.cpp 
  src/HeatMap.cpp 
  src/ForwardDifference.cpp 
  src/Simulation.cpp 
  src/SimulationWindow.cpp 
  src/SimulationToolbar.cpp 
  src/SimulationControlPanel.cpp
)
target_link_libraries(heat_sim
  PRIVATE
  sfml-graphics sfml-system sfml-window
  Eigen3::Eigen
  ImGui-SFML::ImGui-SFML
)
