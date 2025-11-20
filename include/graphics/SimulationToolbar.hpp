#pragma once

#include "Simulation.hpp"

namespace HeatSim {

class SimulationToolbar {

public:
  SimulationToolbar();

  ~SimulationToolbar();

  void display(bool *p_open, Simulation::SimulationState &state);
};

} // namespace HeatSim
