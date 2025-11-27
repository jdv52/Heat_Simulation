#pragma once

#include <functional>
#include <model/AbstractBoundaryCondition.hpp>
#include <vector>

namespace HeatSim {

using forcing_function =
    std::function<double(std::vector<std::size_t>, double)>;

class AbstractPDE {
public:
  AbstractPDE(std::vector<AbstractBoundaryCondition> bcs);
  ~AbstractPDE();

  virtual double evalAt(std::vector<float> x) = 0;
  virtual bool checkIsStable() = 0;

  std::vector<double> computeBCs();

private:
  std::vector<AbstractBoundaryCondition> bcs;
};

} // namespace HeatSim
