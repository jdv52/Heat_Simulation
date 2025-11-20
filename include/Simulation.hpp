#pragma once

#include "PDE.hpp"
#include "PDESolver.hpp"
#include "SimulationCommands.hpp"
#include "util/SPSCDoubleBuffer.hpp"
#include <Eigen/Dense>
#include <chrono>
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

  struct SimulationState {
    long long nIters;
    double simTimeMS;
    double actualUpdatePeriodMS;
    // residual
    // converge warnings
    // const reference to spatial mesh
  };

  using SimRenderPipeline = SPSCDoubleBuffer<Simulation::SimulationState>;

  Simulation(std::shared_ptr<SPSCDoubleBuffer<SimulationState>> sim_out_buff);

  ~Simulation();

  void initSim(const SimulationParam &params);

  bool start();

  void stop();

  void resetState();

private:
  void run(std::stop_token st);

  double source_fn(std::vector<int> x, double t);

  bool isRunning;
  bool isInitialized;
  SimulationParam params;
  SimulationState state;

  Eigen::MatrixXd sourceMesh;

  std::shared_ptr<SimRenderPipeline> simulation_output_buff;
  std::shared_ptr<SPSCDoubleBuffer<SimCommandBase>> command_input_buff;

  std::shared_ptr<PDE::SpatialMesh> shared_mesh_ptr;
  std::unique_ptr<HeatEquationSolver> solver;
  std::unique_ptr<PDE::HeatEquationProblem> heatEq;

  std::jthread driver_thread;
};

} // namespace HeatSim
