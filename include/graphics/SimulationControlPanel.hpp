#pragma once

#include "Simulation.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

namespace HeatSim {

class SimulationControlPanel {

public:
  SimulationControlPanel();

  ~SimulationControlPanel();

  void display(bool *p_open, Simulation::SimulationState &state);

  ImGuiWindowFlags window_flags;
};

} // namespace HeatSim
