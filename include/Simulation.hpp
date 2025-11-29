#pragma once

#include "SimulationConfig.hpp"
#include "SimulationSnapshot.hpp"
#include "model/AbstractPDESolver.hpp"
#include "model/ConvectionDiffusionPDE.hpp"
#include "model/GridRepresentation.hpp"
#include "model/Mesh.hpp"
#include <chrono>

namespace HeatSim {
class Simulation {

public:
  Simulation(SimulationConfig &cfg);
  ~Simulation();

  SimulationSnapshot march();
  double force_fn();

  void sleep_until_next();

private:
  double dt;

  GridRepresentation grid;
  Mesh mesh;
  ConvectionDiffusionPDE pde;

  std::shared_ptr<AbstractPDESolver> solver;

  std::chrono::steady_clock::time_point episodeStartTime;
  std::chrono::steady_clock::time_point iterationStartTime;
  std::chrono::steady_clock::time_point prevUpdateTime;
  long long nIters;
};
} // namespace HeatSim
