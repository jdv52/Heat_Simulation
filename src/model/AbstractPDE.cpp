#include "model/AbstractPDE.hpp"

using namespace HeatSim;

Eigen::VectorXd AbstractPDE::computeBCs() {
  Eigen::VectorXd b;

  // for mesh point in mesh
  //  if mesh point is a boundary condition
  //    if i == 0
  //      set bc to bcs[0]
  //    else if i == numMeshPts - 1
  //      set bc to lastBc
  //    else if i is divisible
  //    compute bc value(need mesh and grid)

  return b;
}

Eigen::SparseMatrix<double> AbstractPDE::getD() { return D; }
