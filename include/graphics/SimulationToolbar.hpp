#pragma once

#include "SimulationSnapshot.hpp"

namespace HeatSim {

class SimulationToolbar {

public:
  SimulationToolbar();

  ~SimulationToolbar();

  void display(bool *p_open, SimulationSnapshot &snapshot);
};

} // namespace HeatSim
