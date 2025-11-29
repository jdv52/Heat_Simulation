#pragma once

#include "AbstractPDE.hpp"
#include "model/AbstractPDE.hpp"
#include "model/GridRepresentation.hpp"
#include "model/Mesh.hpp"
#include <Eigen/SparseCore>

namespace HeatSim {

class ConvectionDiffusionPDE : public AbstractPDE {

public:
  ConvectionDiffusionPDE(GridRepresentation grid, Mesh mesh,
                         double diffusionCoefficient);
  ~ConvectionDiffusionPDE() = default;

private:
  Eigen::SparseMatrix<double> D;
};

} // namespace HeatSim
