#pragma once

#include "PDE.hpp"
#include "PDESolver.hpp"
#include "SimulationCommands.hpp"
#include "SimulationSnapshot.hpp"
#include "util/SPSCDoubleBuffer.hpp"
#include "util/SPSCQueue.hpp"
#include <Eigen/Dense>
#include <chrono>
#include <cstddef>
#include <memory>
#include <thread>

namespace HeatSim {

class Simulation {

public:
  struct SimulationParam {
    std::pair<std::size_t, std::size_t> xyNDivs;
    std::pair<double, double> x_bounds;
    std::pair<double, double> y_bounds;
    double timeStepMS;

    // model parameters
    //    diffusion coefficient
    //    boundary conditions
  };

  using SimRenderPipeline =
      SPSCDoubleBuffer<std::shared_ptr<SimulationSnapshot>>;
  using SimCommandPipeline = SPSCQueue<SimCommandBase *>;

  Simulation(std::shared_ptr<SimRenderPipeline> sim_out_buff,
             std::shared_ptr<SimCommandPipeline>);

  ~Simulation();

  void initSim(const SimulationParam &params);

  bool start();

  void stop();

  void resume();

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

  void singleIteration();

  void transitionTo(SimExecState newState);

  std::string execStateToString(SimExecState state);

  SimExecState execState;
  SimulationParam params;

  Eigen::MatrixXd sourceMesh;

  std::shared_ptr<SimRenderPipeline> simulation_output_buff;
  std::shared_ptr<SimCommandPipeline> command_input_buff;

  std::shared_ptr<PDE::SpatialMesh> shared_mesh_ptr;
  std::unique_ptr<HeatEquationSolver> solver;
  std::unique_ptr<PDE::HeatEquationProblem> heatEq;

  std::jthread driver_thread;

  SimCommandBase *cmd;

  std::chrono::steady_clock::time_point episodeStartTime;
  std::chrono::steady_clock::time_point iterationStartTime;
  std::chrono::steady_clock::time_point prevUpdateTime;

  long long nIters;
};

} // namespace HeatSim
