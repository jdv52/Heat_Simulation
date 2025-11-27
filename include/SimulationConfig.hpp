#pragma once

#include "model/AbstractPDESolver.hpp"
#include <memory>
#include <utility>
#include <vector>

namespace HeatSim {

class SimulationConfig {

public:
  SimulationConfig() = default;
  ~SimulationConfig() = default;

  void setDt(double dt) { _dt = dt; }
  void setDiffusionCoefficient(double diffusionCoefficient) {
    _diffusionCoefficient = diffusionCoefficient;
  }
  void setBounds(std::vector<std::pair<double, double>> bounds) {
    _bounds = bounds;
  }
  void setNDivs(std::vector<std::size_t> nDivs) { _nDivs = nDivs; }

  void setSolver(std::shared_ptr<AbstractPDESolver> solver) {
    _solver = solver;
  }

  double dt() { return _dt; }
  double diffusionCoefficient() { return _diffusionCoefficient; }
  std::vector<std::pair<double, double>> bounds() { return _bounds; }
  std::vector<std::size_t> nDivs() { return _nDivs; }
  std::shared_ptr<AbstractPDESolver> solver() { return _solver; }

private:
  double _dt;
  double _diffusionCoefficient;
  std::vector<std::pair<double, double>> _bounds;
  std::vector<std::size_t> _nDivs;
  std::shared_ptr<AbstractPDESolver> _solver;
};

} // namespace HeatSim
