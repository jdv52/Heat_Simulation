#pragma once

#include <utility>
#include <vector>

namespace HeatSim {

class GridRepresentation {

public:
  GridRepresentation(std::vector<std::pair<double, double>> bounds);
  ~GridRepresentation() = default;

  std::size_t getDimension();

private:
  std::vector<std::pair<double, double>> bounds;
};

} // namespace HeatSim
