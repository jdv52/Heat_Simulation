#include "model/ConvectionDiffusionPDE.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <stdexcept>

using namespace HeatSim;

ConvectionDiffusionPDE::ConvectionDiffusionPDE(GridRepresentation grid,
                                               Mesh mesh,
                                               double diffusionCoefficient)
    : D(mesh.getNumMeshPoints(), mesh.getNumMeshPoints()) {

  auto numMeshPts = mesh.getNumMeshPoints();
  auto dim = grid.getDimension();
  auto bounds = mesh.getDivs();

  std::vector<Eigen::Triplet<double>> elements;
  elements.reserve(numMeshPts + (2 * numMeshPts * dim));

  for (std::size_t i = 0; i < numMeshPts; ++i) {

    elements.push_back(Eigen::Triplet<double>(i, i, -2 * dim));

    std::size_t idx_p = 0;
    std::size_t idx_n = 0;
    for (int j = 0; j < dim; ++j) {
      if (j == 0) {
        idx_n = i - 1;
        idx_p = i + 1;
      } else {
        idx_n -= std::accumulate(bounds.begin(), std::next(bounds.begin(), j),
                                 1, std::multiplies<std::size_t>());
        idx_p += std::accumulate(bounds.begin(), std::next(bounds.begin(), j),
                                 1, std::multiplies<std::size_t>());
      }

      if (!(idx_n < 0 || idx_n >= numMeshPts)) {
        elements.push_back(
            Eigen::Triplet<double>(i, static_cast<int>(idx_n), 1));
      }

      if (!(idx_p < 0 || idx_p >= numMeshPts)) {
        elements.push_back(
            Eigen::Triplet<double>(i, static_cast<int>(idx_p), 1));
      }
    }
  }

  D.setFromTriplets(elements.begin(), elements.end());
}
