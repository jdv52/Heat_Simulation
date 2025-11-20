#include "Simulation.hpp"
#include "graphics/SimulationWindow.hpp"
#include "util/SPSCDoubleBuffer.hpp"
#include <chrono>
#include <csignal>
#include <iostream>

using namespace HeatSim;

volatile std::sig_atomic_t gSignalStatus;

void foo(std::shared_ptr<Simulation::SimRenderPipeline> ptr) {
  while (!gSignalStatus) {
    auto state = ptr->read();

    std::cout << "Number of iterations: " << state.nIters << "\n";
    std::cout << "Sim time: " << state.simTimeMS << "\n";
    std::cout << "Actual Update Period: " << state.actualUpdatePeriodMS << "\n";
    std::cout << "Avg update rate: " << state.nIters / state.simTimeMS * 1000
              << "\n";

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

void signal_handler(int signal) { gSignalStatus = signal; }

int main() {

  signal(SIGINT, signal_handler);

  auto sim_out_buff_ptr = std::make_shared<Simulation::SimRenderPipeline>();
  Simulation sim(sim_out_buff_ptr);
  SimulationWindow sim_win(sim_out_buff_ptr);

  sim.initSim({
      .xyNDivs{10, 10},
      .timeStepMS{100},
  });

  if (!sim.start()) {
    std::cout << "Failed to start simulation!\n";
    exit(-1);
  }

  if (!sim_win.start()) {
    std::cout << "Failed to start simulation window!\n";
    exit(-1);
  }

  while (!gSignalStatus) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  std::cout << "User terminated.\n";
}
