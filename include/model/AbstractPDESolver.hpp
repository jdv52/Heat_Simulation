#pragma once

#include "GridRepresentation.hpp"
#include "model/AbstractPDE.hpp"
#include "model/GridRepresentation.hpp"
#include "model/Mesh.hpp"

namespace HeatSim {

class AbstractPDESolver {
public:
  AbstractPDESolver();
  ~AbstractPDESolver();

  virtual void solve(Mesh &mesh, std::shared_ptr<AbstractPDE> pde,
                     GridRepresentation grid, double dt) = 0;
};

} // namespace HeatSim
