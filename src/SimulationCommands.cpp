#include "SimulationCommands.hpp"
#include "Simulation.hpp"

using namespace HeatSim;

SimToggleCommand::SimToggleCommand() : SimCommandBase::SimCommandBase() {}

SimToggleCommand::~SimToggleCommand() {}

void SimToggleCommand::exec(Simulation &sim) {
  if (!sim.isRunning()) {
    sim.resume();
  } else {
    sim.stop();
  }
}
