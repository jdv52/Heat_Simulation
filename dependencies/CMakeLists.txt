include(FetchContent)

FetchContent_Declare(
  SFML
  GIT_REPOSITORY "https://github.com/SFML/SFML.git"
  GIT_TAG  3.0.x
)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
  GIT_TAG  v1.91.1
  GIT_PROGRESS TRUE
)

FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY "https://github.com/SFML/imgui-sfml.git"
  GIT_TAG  v3.0
  GIT_PROGRESS TRUE
)

FetchContent_Declare(
  eigen
  GIT_REPOSITORY "https://gitlab.com/libeigen/eigen"
  GIT_TAG  5.0.0
  GIT_PROGRESS TRUE
)

FetchContent_Declare(
  argparse
  GIT_REPOSITORY "https://github.com/p-ranav/argparse"
  GIT_TAG  v3.2
  GIT_PROGRESS TRUE
)

FetchContent_Declare(
  inicpp
  GIT_REPOSITORY "https://github.com/Rookfighter/inifile-cpp"
  GIT_PROGRESS TRUE
)

# SFML
option(SFML_BUILD_AUDIO "Build audio" OFF)
option(SFML_BUILD_NETWORK "Build network" OFF)
FetchContent_MakeAvailable(sfml)

# Dear ImGui
FetchContent_MakeAvailable(imgui)

# ImGui-SFML
set(IMGUI_DIR ${imgui_SOURCE_DIR})
option(IMGUI_SFML_FIND_SFML "Use find_package to find SFML" OFF)
# option(IMGUI_SFML_IMGUI_DEMO "Build imgui_demo.cpp" ON)

FetchContent_MakeAvailable(imgui-sfml)

# Eigen
FetchContent_MakeAvailable(eigen)

# argparse
FetchContent_MakeAvailable(argparse)

#inifile
FetchContent_MakeAvailable(inicpp)
