#include "model/Mesh.hpp"
#include <iostream>
#include <numeric>
#include <stdexcept>

using namespace HeatSim;

Mesh::Mesh(GridRepresentation grid, std::vector<std::size_t> _nDivs)
    : nDivs(_nDivs) {
  if (grid.getDimension() != nDivs.size()) {
    throw std::logic_error("Grid topology dimension is inconsistent with "
                           "dimension of specified divisions.");
  }

  std::size_t meshSize =
      std::accumulate(nDivs.begin(), nDivs.end(), 1, std::multiplies<int>());
  std::cout << "Allocating a mesh of size " << meshSize << "\n";

  mesh = Eigen::VectorXd(meshSize);
}

Eigen::VectorXd &Mesh::getMesh() { return mesh; }
