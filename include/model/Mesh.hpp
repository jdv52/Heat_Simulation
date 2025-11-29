#pragma once

#include "model/GridRepresentation.hpp"
#include <Eigen/Dense>

namespace HeatSim {

class Mesh {
public:
  Mesh(GridRepresentation grid, std::vector<std::size_t> nDivs);
  ~Mesh() = default;

  Eigen::VectorXd &getMesh();
  std::size_t getNumMeshPoints();
  std::vector<std::size_t> getDivs();

private:
  std::vector<std::size_t> nDivs;
  Eigen::VectorXd mesh;
};

} // namespace HeatSim
