#pragma once

#include <Eigen/Dense>
#include <Eigen/SparseCore>
#include <functional>
#include <model/AbstractBoundaryCondition.hpp>
#include <vector>

namespace HeatSim {

using forcing_function =
    std::function<double(std::vector<std::size_t>, double)>;

class AbstractPDE {
public:
  AbstractPDE() = default;
  ~AbstractPDE() = default;

  // virtual bool checkIsStable() = 0;

  Eigen::VectorXd computeBCs();

  Eigen::SparseMatrix<double> getD();

protected:
  std::vector<AbstractBoundaryCondition> bcs;
  Eigen::SparseMatrix<double> D;
};

} // namespace HeatSim
