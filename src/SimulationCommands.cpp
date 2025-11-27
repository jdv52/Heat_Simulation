#include "SimulationCommands.hpp"
#include "SimulationManager.hpp"

using namespace HeatSim;

SimToggleCommand::SimToggleCommand() : SimCommandBase::SimCommandBase() {}

SimToggleCommand::~SimToggleCommand() {}

void SimToggleCommand::exec(SimulationManager &sim) {
  if (!sim.isRunning()) {
    sim.resume();
  } else {
    sim.stop();
  }
}
