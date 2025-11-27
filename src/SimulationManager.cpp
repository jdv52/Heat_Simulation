#include "SimulationManager.hpp"
#include "SimulationSnapshot.hpp"
#include <chrono>
#include <iostream>
#include <stdexcept>

using namespace HeatSim;

SimulationManager::SimulationManager(
    std::shared_ptr<SimRenderPipeline> sim_out_buff,
    std::shared_ptr<SimCommandPipeline> cmd_pipe)
    : execState{SimExecState::uninitialized},
      simulation_output_buff(sim_out_buff), command_input_buff(cmd_pipe) {}

SimulationManager::~SimulationManager() {}

bool SimulationManager::start() {
  if (!driver_thread.joinable()) {
    transitionTo(SimExecState::startingRun);

    driver_thread = std::jthread{[this](std::stop_token st) { this->run(st); }};
    return true;
  } else {
    std::cout << "Simulation has already been started!\n";
    return false;
  }
}

void SimulationManager::stop() {
  transitionTo(SimExecState::stoppingRun);
  std::cout << "Simulation stopping.\n";
}

void SimulationManager::resume() {
  transitionTo(SimExecState::startingRun);
  std::cout << "Simulation resuming.\n";
}

void SimulationManager::pause() { std::cout << "Simulation pausing.\n"; }

bool SimulationManager::isRunning() {
  return execState == SimExecState::running;
}

void SimulationManager::loadConfig(const SimulationConfig &_param) {
  config = _param;

  execState = SimExecState::initialized;
}

void SimulationManager::transitionTo(SimExecState newState) {
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

std::string SimulationManager::execStateToString(SimExecState _state) {
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

/*
void SimulationManager::singleIteration() {

}
*/

double SimulationManager::source_fn(std::vector<std::size_t>, double) {
  return 0.0;
}

void SimulationManager::run(std::stop_token st) {
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
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      break;
    case SimExecState::startingRun:
      std::cout << "Starting simulation episode.\n";
      sim.reset(new Simulation(config));
      execState = SimExecState::running;

      break;
    case SimExecState::running: {
      auto snapshot = sim->march();

      simulation_output_buff->write(
          std::make_shared<SimulationSnapshot>(snapshot));

      sim->sleep_until_next();
      break;
    }
    case SimExecState::stoppingRun:
      // clear the board
      transitionTo(SimExecState::idle);
      // output a snapshot
      break;
    }
  }

  std::cout << "Simulation thread terminated.\n";
}
