#include "model/GridRepresentation.hpp"
#include <iostream>

using namespace HeatSim;

GridRepresentation::GridRepresentation(
    std::vector<std::pair<double, double>> _bounds)
    : bounds{_bounds} {
  std::cout << "Created a grid with the following bounds: \n";
  for (auto bound : _bounds) {
    std::cout << "\t(" << std::get<0>(bound) << ", " << std::get<1>(bound)
              << ")\n";
  }
}

std::size_t GridRepresentation::getDimension() { return bounds.size(); }
