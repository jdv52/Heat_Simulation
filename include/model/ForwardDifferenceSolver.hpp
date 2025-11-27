#pragma once

#include "model/AbstractPDESolver.hpp"

namespace HeatSim {

class ForwardDifference : private AbstractPDESolver {

  ForwardDifference();
  ~ForwardDifference();

  virtual void solve(Mesh &mesh, std::shared_ptr<AbstractPDE> pde,
                     GridRepresentation grid, double dt) override;
};
} // namespace HeatSim
