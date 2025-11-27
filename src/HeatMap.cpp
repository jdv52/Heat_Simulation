#include <algorithm>
#include <ctime>
#include <functional>
#include <iostream>

#include "HeatMap.hpp"

HeatMap::HeatMap(std::size_t _stepSize, std::size_t _mapSize,
                 sf::RenderWindow &ctx, HeatMapGradient gradient)
    : stepSize{_stepSize}, mapSize{_mapSize}, window_ctx{ctx},
      gradient(gradient) {}

HeatMap::~HeatMap() {}

/*
void HeatMap::setGradient(HeatMapGradient &_gradient) {
  this->gradient = &_gradient;
}
*/

/*
void HeatMap::applyHeatAtPoint(int x, int y, int kernelSize, double strength,
                               double heatMultiplier) {
  double sum = 0;
  double sigma = 1 / strength;
  int center = kernelSize / 2;

  double kernel[kernelSize][kernelSize];

  for (int i = 0; i < kernelSize; ++i) {
    for (int j = 0; j < kernelSize; ++j) {
      int _x = i - center;
      int _y = j - center;
      double val = exp(-(_x * _x + _y * _y) / (2 * sigma * sigma)) /
                   (2 * M_PI * sigma * sigma);

      kernel[i][j] = val;

      sum += val;
    }
  }

  for (int i = 0; i < kernelSize; ++i) {
    for (int j = 0; j < kernelSize; ++j) {
      int matrixY = i - center + x;
      int matrixX = j - center + y;

      if (matrixX > 0 && matrixX < mapSize - 1 && matrixY > 0 &&
          matrixY < mapSize - 1)
        drawingMatrix(matrixX, matrixY) += kernel[i][j] * heatMultiplier /
sum;
    }
  }
}
*/

void HeatMap::draw(PDE::SpatialMesh &mesh) {

  for (std::size_t i = 0; i < mapSize; ++i) {
    for (std::size_t j = 0; j < mapSize; ++j) {
      auto cell_rect = sf::RectangleShape(sf::Vector2f(
          static_cast<float>(stepSize), static_cast<float>(stepSize)));
      cell_rect.setPosition(
          sf::Vector2f(static_cast<float>(i * stepSize) + 0.0f,
                       static_cast<float>(j * stepSize) + 0.0f));
      cell_rect.setOutlineThickness(1.0);
      cell_rect.setOutlineColor(sf::Color::Black);

      double temp = mesh.getFValAtMeshPoint(std::vector<std::size_t>({i, j}));
      cell_rect.setFillColor(gradient.mapdoubleToColor(temp, -1000, 1000));
      window_ctx.draw(cell_rect);
    }
  }
}

/*
void HeatMap::initPDE() {
  PDE::Function_handle fn = [this](std::vector<int> x, double t) {
    return this->source_fn(x, t);
  };

  heatEq =
      std::make_unique<PDE::HeatEquationProblem>(1e-2, shared_mesh_ptr, fn);
}
*/

/*
void HeatMap::simulate_ManualStep() {
  solver->solve(*heatEq);
  clearDrawing();
}
*/

/*
double HeatMap::getPointOnDrawing(int i, int j) { return drawingMatrix(i, j);
}
*/

/*
void HeatMap::clearDrawing() { drawingMatrix.setZero(); }
*/
