#include "Simulation.hpp"
#include "graphics/SimulationWindow.hpp"
#include "util/SPSCDoubleBuffer.hpp"
#include <chrono>
#include <csignal>
#include <iostream>

using namespace HeatSim;

volatile std::sig_atomic_t gSignalStatus;

void signal_handler(int signal) { gSignalStatus = signal; }

int main() {

  signal(SIGINT, signal_handler);

  auto sim_out_buff_ptr = std::make_shared<Simulation::SimRenderPipeline>();
  auto sim_cmd_buff_ptr = std::make_shared<Simulation::SimCommandPipeline>(100);

  Simulation sim(sim_out_buff_ptr, sim_cmd_buff_ptr);
  SimulationWindow sim_win(sim_out_buff_ptr, sim_cmd_buff_ptr);

  sim.initSim(
      {.xyNDivs{10, 10}, .x_bounds{-1, 1}, .y_bounds{-1, 1}, .timeStepMS{10}});

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
