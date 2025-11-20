#include "Simulation.hpp"
#include <chrono>
#include <iostream>
#include <stdexcept>

using namespace HeatSim;

Simulation::Simulation(
    std::shared_ptr<SPSCDoubleBuffer<SimulationState>> sim_out_buff)
    : isInitialized{false}, isRunning{false},
      simulation_output_buff(sim_out_buff) {}

Simulation::~Simulation() {}

bool Simulation::start() {
  if (!isInitialized) {
    std::cout << "Simulation not initialized! Thread not started.\n";
    return false;
  } else if (!driver_thread.joinable()) {
    driver_thread = std::jthread{[this](std::stop_token st) { this->run(st); }};
    return true;
  } else {
    return false;
    std::cout << "Simulation has already been started!\n";
  }
}

void Simulation::stop() {}

void Simulation::initSim(const Simulation::SimulationParam &_param) {
  params = _param;

  shared_mesh_ptr.reset(new PDE::SpatialMesh{std::vector<double>{2, 1.0}, 10});
  solver.reset(new ForwardDifference(params.timeStepMS));
  heatEq.reset(new PDE::HeatEquationProblem(
      1e-2, shared_mesh_ptr,
      [this](std::vector<int> x, double t) { return this->source_fn(x, t); }));

  resetState();

  isInitialized = true;
}

void Simulation::resetState() { state.nIters = 0; }

double Simulation::source_fn(std::vector<int> x, double t) { return 0.0; }

void Simulation::run(std::stop_token st) {
  std::cout << "Started simulation thread.\n";

  // episode start time
  auto episodeStartTime = std::chrono::system_clock::now();
  auto prevUpdateTime = episodeStartTime;

  while (!st.stop_requested()) {
    // compute iteration start time
    auto iterationStartTime = std::chrono::system_clock::now();

    // poll for input in the queue
    // apply command

    solver->solve(*heatEq);

    // compute time elapsed between now and episode start time as simulation
    // time
    // compute time elapsed between iteration start time and prev_start time as
    // actual period
    state.nIters++;

    auto tNow = std::chrono::system_clock::now();

    auto simTimeDuration =
        std::chrono::duration_cast<std::chrono::milliseconds>(tNow -
                                                              episodeStartTime);
    state.simTimeMS = simTimeDuration.count();

    auto updatePeriodDuration =
        std::chrono::duration_cast<std::chrono::microseconds>(tNow -
                                                              prevUpdateTime);
    state.actualUpdatePeriodMS = updatePeriodDuration.count() / 1000.0;

    prevUpdateTime = tNow;

    simulation_output_buff->write(state);

    // wait until iteration start time + deltaTime
    std::this_thread::sleep_until(
        iterationStartTime +
        std::chrono::microseconds((int)params.timeStepMS * 1000));
  }

  std::cout << "Simulation thread termianted.\n";
}
