#pragma once

#include "SimulationSnapshot.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

namespace HeatSim {

class SimulationControlPanel {

public:
  SimulationControlPanel();

  ~SimulationControlPanel();

  void display(bool *p_open, SimulationSnapshot &snapshot);

  ImGuiWindowFlags window_flags;
};

} // namespace HeatSim
