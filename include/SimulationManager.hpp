#pragma once

#include "PDE.hpp"
#include "PDESolver.hpp"
#include "Simulation.hpp"
#include "SimulationCommands.hpp"
#include "SimulationConfig.hpp"
#include "SimulationSnapshot.hpp"
#include "util/SPSCDoubleBuffer.hpp"
#include "util/SPSCQueue.hpp"
#include <Eigen/Dense>
#include <chrono>
#include <cstddef>
#include <memory>
#include <thread>

namespace HeatSim {

class SimulationManager {

public:
  using SimRenderPipeline =
      SPSCDoubleBuffer<std::shared_ptr<SimulationSnapshot>>;
  using SimCommandPipeline = SPSCQueue<SimCommandBase *>;

  SimulationManager(std::shared_ptr<SimRenderPipeline> sim_out_buff,
                    std::shared_ptr<SimCommandPipeline>);

  ~SimulationManager();

  void loadConfig(const SimulationConfig &params);

  bool start();

  void stop();

  void resume();

  void pause();

  void processCommands();

  bool isRunning();

private:
  enum class SimExecState {
    uninitialized,
    initialized,
    idle,
    startingRun,
    running,
    stoppingRun
  };

  void run(std::stop_token st);

  double source_fn(std::vector<std::size_t> x, double t);

  void transitionTo(SimExecState newState);

  std::string execStateToString(SimExecState state);

  SimExecState execState;
  SimulationConfig config;

  Eigen::MatrixXd sourceMesh;

  std::shared_ptr<SimRenderPipeline> simulation_output_buff;
  std::shared_ptr<SimCommandPipeline> command_input_buff;

  std::jthread driver_thread;

  SimCommandBase *cmd;

  std::unique_ptr<Simulation> sim;
  /*
   */
};

} // namespace HeatSim
