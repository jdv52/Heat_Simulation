#pragma once

#include "GridRepresentation.hpp"
#include "Mesh.hpp"

namespace HeatSim {
class AbstractBoundaryCondition {
  AbstractBoundaryCondition();
  ~AbstractBoundaryCondition();

  virtual double
  computeBoundaryValue(double dt, std::vector<std::size_t> mesh_pt, Mesh mesh);
};
} // namespace HeatSim
