#include "vector"
#include <iostream>

#include "PDESolver.hpp"
#include "eigen3/Eigen/SparseCore"

ForwardDifference::ForwardDifference(float dt)
    : HeatEquationSolver(dt)
{

}

ForwardDifference::~ForwardDifference()
{

}

void ForwardDifference::solve(PDE::HeatEquationProblem& heatEq)
{
    // TODO: Needs HELLA optimization:
    //       - Can we move computation of sigma and the stencil somewhere else so we only have to do it once?
    //       - Can we have a function that just copies over the solution vector directly to the domain?
    //       As for non-optimizing actions:
    //       - Need to integrate boundary conditions and source function
    int spatialDivs = (heatEq.getDomainPtr())->getNumDivs();
    float sigma = heatEq.getDifussionCoefficient() * (float)timeStep * float(spatialDivs) * float(spatialDivs);
    // std::cout << "Sigma was: " << sigma << "\n";
    // if (sigma > 0.5)
    // {
    //     std::cout << "Warning: sigma value > 0.5. FDM method may be unstable.";
    // }

    Eigen::MatrixXf wij = Eigen::Map<Eigen::MatrixXf>(((heatEq.getDomainPtr())->getDomainAsVectorPtr())->data(), spatialDivs, spatialDivs);
    wij.resize(spatialDivs * spatialDivs, 1);

    Eigen::SparseMatrix<float> stencil (spatialDivs * spatialDivs, spatialDivs * spatialDivs);
    Eigen::SparseMatrix<float> bcs (spatialDivs * spatialDivs, spatialDivs * spatialDivs);

    std::vector<Eigen::Triplet<float>> coeffTriplets;
    for (int i = 1; i < spatialDivs - 1; ++i)
    {
        for (int j = 1; j < spatialDivs - 1; ++j)
        {
            coeffTriplets.push_back(Eigen::Triplet<float>(i * spatialDivs + j, i * spatialDivs + j, 0.5 - (2 * sigma)));
            coeffTriplets.push_back(Eigen::Triplet<float>(i * spatialDivs + j, i * spatialDivs + j + 1, sigma));
            coeffTriplets.push_back(Eigen::Triplet<float>(i * spatialDivs + j, i * spatialDivs + j - 1, sigma));
            coeffTriplets.push_back(Eigen::Triplet<float>(i * spatialDivs + j, i * spatialDivs + j + spatialDivs, sigma));
            coeffTriplets.push_back(Eigen::Triplet<float>(i * spatialDivs + j, i * spatialDivs + j - spatialDivs, sigma));
        }
    }
    stencil.setFromTriplets(coeffTriplets.begin(), coeffTriplets.end());

    std::vector<Eigen::Triplet<float>> bcsTriplets;
    for (int i = 0; i < spatialDivs; ++ i)
    {
        for (int j = 0; j < spatialDivs; ++ j) {
            float bc = 1;
            if (i == 0) {
                bcsTriplets.push_back(Eigen::Triplet<float>(i * spatialDivs + j, i * spatialDivs + j + spatialDivs, bc));
            }
            if (i == spatialDivs - 1)
                bcsTriplets.push_back(Eigen::Triplet<float>(i * spatialDivs + j, i * spatialDivs + j - spatialDivs, bc));

            if (j == 0) {
                bcsTriplets.push_back(Eigen::Triplet<float>(i * spatialDivs + j, i * spatialDivs + j + 1, bc));
            }
            if (j == spatialDivs - 1)
                bcsTriplets.push_back(Eigen::Triplet<float>(i * spatialDivs + j, i * spatialDivs + j - 1, bc));
        }
        
    }

    Eigen::MatrixXf F (spatialDivs * spatialDivs, 1);
    for (int i = 0; i < spatialDivs; ++i)
    {
        for (int j = 0; j < spatialDivs; ++j)
        {

            F(spatialDivs * i + j, 0) = heatEq.evaluateSource(i, j);
        }
    }

    bcs.setFromTriplets(bcsTriplets.begin(), bcsTriplets.end());

    Eigen::MatrixXf wijp1 = ((stencil + bcs) * wij) + F;

    for (int i = 0; i < spatialDivs * spatialDivs; ++i)
    {
        (heatEq.getDomainPtr())->setFValAtIdx(i, wijp1(i , 0));
    }

    // std::cout << stencil << "\n";
    // std::cout << bcs << "\n";
    wijp1.resize(spatialDivs, spatialDivs);
    // std::cout << wijp1 << "\n\n";
}