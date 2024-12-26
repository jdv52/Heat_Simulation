#include "vector"
#include <iostream>

#include "PDESolver.hpp"
#include "eigen3/Eigen/SparseCore"

ForwardDifference::ForwardDifference(float dt)
    : HeatEquationSolver(dt)
{

}

void ForwardDifference::solve(PDE::HeatEquationProblem& heatEq)
{
    float spatialDivs = (heatEq.getDomainPtr())->getNumDivs();
    float sigma = heatEq.getDifussionCoefficient() * (float)timeStep * float(spatialDivs) * float(spatialDivs);

    Eigen::MatrixXf wij = Eigen::Map<Eigen::MatrixXf>(((heatEq.getDomainPtr())->getDomainAsVectorPtr())->data(), spatialDivs, spatialDivs);
    wij.resize(spatialDivs * spatialDivs, 1);

    Eigen::SparseMatrix<float> stencil (spatialDivs * spatialDivs, spatialDivs * spatialDivs);

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


    Eigen::MatrixXf wijp1 = stencil * wij;

    for (int i = 1; i < spatialDivs * spatialDivs - 2; ++i)
    {
        (heatEq.getDomainPtr())->setFValAtIdx(i, wijp1(i , 0));
    }

    wijp1.resize(spatialDivs, spatialDivs);
}