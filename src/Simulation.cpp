#include "Simulation.hpp"
#include "SimulationSnapshot.hpp"
#include <chrono>
#include <iostream>
#include <stdexcept>

using namespace HeatSim;

Simulation::Simulation(std::shared_ptr<SimRenderPipeline> sim_out_buff,
                       std::shared_ptr<SimCommandPipeline> cmd_pipe)
    : execState{SimExecState::uninitialized},
      simulation_output_buff(sim_out_buff), command_input_buff(cmd_pipe) {}

Simulation::~Simulation() {}

bool Simulation::start() {
  if (!driver_thread.joinable()) {
    transitionTo(SimExecState::startingRun);

    driver_thread = std::jthread{[this](std::stop_token st) { this->run(st); }};
    return true;
  } else {
    return false;
    std::cout << "Simulation has already been started!\n";
  }
}

void Simulation::stop() {
  transitionTo(SimExecState::stoppingRun);
  std::cout << "Simulation stopping.\n";
}

void Simulation::resume() {
  transitionTo(SimExecState::startingRun);
  std::cout << "Simulation stopping.\n";
}

bool Simulation::isRunning() { return execState == SimExecState::running; }

void Simulation::initSim(const Simulation::SimulationParam &_param) {
  params = _param;

  shared_mesh_ptr.reset(new PDE::SpatialMesh{std::vector<double>{2, 1.0}, 20});
  solver.reset(new ForwardDifference(params.timeStepMS));
  heatEq.reset(new PDE::HeatEquationProblem(
      1e-2, shared_mesh_ptr, [this](std::vector<std::size_t> x, double t) {
        return this->source_fn(x, t);
      }));

  nIters = 0;

  execState = SimExecState::initialized;
}

void Simulation::transitionTo(SimExecState newState) {
  auto illegalTransitionMsg =
      std::format("Illegal state transition: {} -> {} ",
                  execStateToString(execState), execStateToString(newState));

  switch (execState) {
  case SimExecState::uninitialized:
    if (newState != SimExecState::initialized)
      throw std::logic_error(illegalTransitionMsg);
    break;
  case SimExecState::initialized:
    if (newState != SimExecState::idle && newState != SimExecState::startingRun)
      throw std::logic_error(illegalTransitionMsg);
    break;
  case SimExecState::startingRun:
    if (newState != SimExecState::running &&
        newState != SimExecState::stoppingRun)
      throw std::logic_error(illegalTransitionMsg);
    break;
  case SimExecState::running:
    if (newState != SimExecState::stoppingRun)
      throw std::logic_error(illegalTransitionMsg);
    break;
  case SimExecState::stoppingRun:
    if (newState != SimExecState::idle)
      throw std::logic_error(illegalTransitionMsg);
    break;
  case SimExecState::idle:
    if (newState == SimExecState::running &&
        newState == SimExecState::stoppingRun)
      throw std::logic_error(illegalTransitionMsg);
    break;
  default:
    throw std::logic_error("Unknown state transition");
  }

  execState = newState;
}

std::string Simulation::execStateToString(SimExecState _state) {
  switch (_state) {
  case SimExecState::uninitialized:
    return "uninitialized";
  case SimExecState::initialized:
    return "initialized";
  case SimExecState::startingRun:
    return "startingRun";
  case SimExecState::running:
    return "running";
  case SimExecState::stoppingRun:
    return "stoppingRun";
  case SimExecState::idle:
    return "idle";
  }
  return "";
}

void Simulation::singleIteration() {
  auto start = std::chrono::steady_clock::now();

  solver->solve(*heatEq);

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

  simulation_output_buff->write(std::make_shared<SimulationSnapshot>(
      isRunning(), nIters, simTimeMS, actualUpdatePeriodMS,
      algorithmCompletionTimeMS, *shared_mesh_ptr));

  prevUpdateTime = start;

  iterationStartTime +=
      std::chrono::microseconds((int)(params.timeStepMS * 1000));

  // wait until iteration start time + deltaTime
  std::this_thread::sleep_until(iterationStartTime);
}

double Simulation::source_fn(std::vector<std::size_t>, double) { return 0.0; }

void Simulation::run(std::stop_token st) {
  std::cout << "Started simulation thread.\n";

  while (!st.stop_requested()) {
    // poll for input in the queue
    if (command_input_buff->pop(cmd)) {
      cmd->exec(*this);
      delete cmd;
    }

    switch (execState) {
    case SimExecState::uninitialized:
    case SimExecState::initialized:
      throw std::logic_error("Simulation was incorrectly initialized");
      break;
    case SimExecState::idle:
      // do nothing
      break;
    case SimExecState::startingRun:
      std::cout << "Starting simulation episode.\n";

      episodeStartTime = std::chrono::steady_clock::now();
      iterationStartTime = episodeStartTime;
      prevUpdateTime = episodeStartTime;
      execState = SimExecState::running;

      break;
    case SimExecState::running:
      singleIteration();
      break;
    case SimExecState::stoppingRun:
      // clear the board
      transitionTo(SimExecState::idle);
      // output a snapshot
      nIters = 0;
      break;
    }
  }

  std::cout << "Simulation thread terminated.\n";
}
