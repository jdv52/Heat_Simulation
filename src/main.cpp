#include "ConfigManager.hpp"
#include "SimulationConfig.hpp"
#include "SimulationManager.hpp"
#include "graphics/SimulationWindow.hpp"
#include "util/SPSCDoubleBuffer.hpp"
#include <argparse/argparse.hpp>
#include <chrono>
#include <csignal>
#include <iostream>

using namespace HeatSim;

volatile std::sig_atomic_t gSignalStatus;

void signal_handler(int signal) { gSignalStatus = signal; }

int main(int argc, char *argv[]) {

  argparse::ArgumentParser program("heat_simulation");
  program.add_argument("-c", "--cfg_file")
      .help("default config file")
      .required();

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    std::exit(-1);
  }

  signal(SIGINT, signal_handler);

  ConfigManager cfg_mgr;
  cfg_mgr.loadConfigFile(program.get<std::string>("-c"));
  SimulationConfig cfg = cfg_mgr.getConfig();

  auto sim_out_buff_ptr =
      std::make_shared<SimulationManager::SimRenderPipeline>();
  auto sim_cmd_buff_ptr =
      std::make_shared<SimulationManager::SimCommandPipeline>(100);

  SimulationManager sim(sim_out_buff_ptr, sim_cmd_buff_ptr);
  SimulationWindow sim_win(sim_out_buff_ptr, sim_cmd_buff_ptr);

  /*
  sim.loadConfig(
      {.xyNDivs{10, 10}, .x_bounds{-1, 1}, .y_bounds{-1, 1}, .timeStepMS{10}});
  */
  sim.loadConfig(cfg);

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
