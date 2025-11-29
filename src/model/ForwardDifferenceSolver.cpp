#include "model/ForwardDifferenceSolver.hpp"

using namespace HeatSim;

ForwardDifference::ForwardDifference() {}

ForwardDifference::~ForwardDifference() {}

void ForwardDifference::solve(Mesh &mesh, std::shared_ptr<AbstractPDE> pde,
                              GridRepresentation grid, double dt) {

  double sigma = dt / mesh.getNumMeshPoints();

  auto I = Eigen::MatrixXd::Identity(mesh.getNumMeshPoints(),
                                     mesh.getNumMeshPoints());

  auto D = pde->getD();
  auto b = pde->computeBCs();

  // Compute forcing function

  auto wijp1 = (I - sigma * D) * mesh.getMesh(); // + F + b
}
