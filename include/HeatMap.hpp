#pragma once

#include "Eigen/Dense"
#include <SFML/Graphics.hpp>
#include <cstddef>
#include <mutex>
#include <thread>
#include <vector>

#include "HeatMapGradient.hpp"
#include "model/Mesh.hpp"

namespace HeatSim {

class HeatMap {
public:
  HeatMap(std::size_t stepSize, std::size_t mapSize, sf::RenderWindow &ctx,
          HeatMapGradient gradient);
  ~HeatMap();

  void setGradient(HeatMapGradient &gradient);
  void draw(Mesh &mesh);

private:
  std::size_t stepSize, mapSize;
  sf::RenderWindow &window_ctx;
  HeatMapGradient &gradient;

  void initMap();
};

} // namespace HeatSim
