#include "Simulation.hpp"
#include <chrono>
#include <thread>

using namespace HeatSim;

Simulation::Simulation(SimulationConfig &cfg)
    : grid(cfg.bounds()), mesh(grid, cfg.nDivs()) {
  nIters = 0;
  episodeStartTime = std::chrono::steady_clock::now();
  iterationStartTime = episodeStartTime;
  prevUpdateTime = episodeStartTime;

  dt = cfg.dt();

  // Construct Boundary Conditions
  // Get solver from parameters
  // Construct a PDE
}

Simulation::~Simulation() {}

SimulationSnapshot Simulation::march() {
  auto start = std::chrono::steady_clock::now();

  // solver->solve(*heatEq);

  auto stop = std::chrono::steady_clock::now();

  // Compute timing information
  auto iterationPeriod =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  auto simTimeDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
      start - episodeStartTime);
  auto updatePeriodDuration =
      std::chrono::duration_cast<std::chrono::microseconds>(start -
                                                            prevUpdateTime);

  // Update output state
  auto algorithmCompletionTimeMS =
      static_cast<double>(iterationPeriod.count()) / 1000.0;
  auto simTimeMS = static_cast<double>(simTimeDuration.count());
  auto actualUpdatePeriodMS =
      static_cast<double>(updatePeriodDuration.count()) / 1000.0;
  nIters++;

  prevUpdateTime = start;

  iterationStartTime += std::chrono::microseconds((int)(dt * 1000));

  SimulationSnapshot snapshot(true, nIters, simTimeMS, actualUpdatePeriodMS,
                              algorithmCompletionTimeMS);
  return snapshot;
}

void Simulation::sleep_until_next() {
  std::this_thread::sleep_until(iterationStartTime);
}
