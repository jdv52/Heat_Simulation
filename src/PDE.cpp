#include "PDE.hpp"

#include <cmath>
#include <stdexcept>
#include <iostream>
#include <iomanip>

namespace PDE {
    SpatialMesh::SpatialMesh(std::vector<double> _bounds, int _nDivs)
    {
        bounds = _bounds;
        nDivs = _nDivs;
        dimension = _bounds.size();

        mesh = std::vector<double>(std::pow(nDivs, dimension), 0.0);
    }

    int SpatialMesh::getNumDivs()
    {
        return nDivs;
    }

    void SpatialMesh::printMesh()
    {
        for (int i = 0; i < mesh.size(); ++i)
        {
            std::cout << i << " = (";
            std::vector<int> meshPoints = vectorIdxToMeshPoint(i);

            for (int j = 0; j < meshPoints.size(); ++j)
            {
                int point = meshPoints.at(j);
                double coordPoint = point * bounds.at(j) / nDivs;
                std::cout << std::fixed << std::setprecision(2) << coordPoint;
                if (j != meshPoints.size() - 1)
                {
                    std::cout << ", ";
                }
            }
            std::cout << ") = " << getFValAtIdx(i) << " ";

            if ((i + 1) % nDivs == 0)
            {
                std::cout << "\n";
            }
        }
        std::cout << "\n";
    }

    void SpatialMesh::setFValAtMeshPoint(std::vector<int> meshPoint, double val)
    {
        int idx = meshPointToVectorIdx(meshPoint);

        mesh.at(idx) = val;
    }

    double SpatialMesh::getFValAtMeshPoint(std::vector<int> meshPoint)
    {
        int idx = meshPointToVectorIdx(meshPoint);
        return mesh.at(idx);
    }

    void SpatialMesh::setFValAtIdx(int idx, double val)
    {
        mesh.at(idx) = val;
    }

    double SpatialMesh::getFValAtIdx(int idx)
    {
        return mesh.at(idx);
    }

    int SpatialMesh::meshPointToVectorIdx(std::vector<int> meshPoint)
    {
        if (meshPoint.size() == dimension)
        {
            int idx = 0;
            for (int i = 0; i < dimension; ++i)
            {
                idx += meshPoint.at(i) * std::pow(nDivs, i);
            }

            return idx;
        } else {
            throw std::runtime_error("Input meshPoints dimension is inconsistent with mesh dimension.");
        }
    }

    std::vector<int> SpatialMesh::vectorIdxToMeshPoint(int vectorIdx)
    {
        std::vector<int> meshPoints;
        for (int i = 0; i < dimension; ++i)
        {
            meshPoints.push_back(vectorIdx % nDivs);
            vectorIdx /= nDivs;
        }

        return meshPoints;
    }

    HeatEquationProblem::HeatEquationProblem(double diffusionCoefficient, std::shared_ptr<SpatialMesh>spatialDomain, Function_handle _source)
        : diffusionCoefficient(diffusionCoefficient),
        domain(spatialDomain),
        source(_source)
    {
    }

    HeatEquationProblem::~HeatEquationProblem()
    {
        
    }

    double HeatEquationProblem::getDifussionCoefficient()
    {
        return diffusionCoefficient;
    }

    double HeatEquationProblem::evaluateSource(int i, int j)
    {
        return source(std::vector<int>({i, j}), 0);
    }

    std::shared_ptr<SpatialMesh> HeatEquationProblem::getDomainPtr()
    {
        return domain;
    }

    std::vector<double>* SpatialMesh::getDomainAsVectorPtr()
    {
        return &mesh;
    }
}
