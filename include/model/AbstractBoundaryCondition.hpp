#pragma once

#include "GridRepresentation.hpp"
#include "Mesh.hpp"

namespace HeatSim {
class AbstractBoundaryCondition {

public:
  AbstractBoundaryCondition() = default;
  ~AbstractBoundaryCondition() = default;

  virtual double
  computeBoundaryValue(double dt, std::vector<std::size_t> mesh_pt, Mesh mesh);
};
} // namespace HeatSim
