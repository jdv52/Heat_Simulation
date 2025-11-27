#include "model/ForwardDifferenceSolver.hpp"

using namespace HeaetSim;

ForwardDifference::ForwardDifference() {}

ForwardDifference::~ForwardDifference() {}

void ForwardDifference::solve(Mesh &mesh, std::shared_ptr<AbstractPDE> pde,
                              GridRepresentation grid, double dt) {
  // Create tri-diagonal differential operator from PDE

  // Compute boundary conditions

  // Compute forcing function

  // wijp1 = (I - sigma * D) * wij + F
}
