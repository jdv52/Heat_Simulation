#include "PDE.hpp"

#include <cmath>
#include <cstddef>
#include <format>
#include <iomanip>
#include <iostream>
#include <stdexcept>

namespace PDE {
SpatialMesh::SpatialMesh(std::vector<double> _bounds, std::size_t _nDivs) {
  bounds = _bounds;
  nDivs = _nDivs;
  dimension = _bounds.size();

  auto meshCount = static_cast<std::size_t>(std::pow(nDivs, dimension));
  mesh = std::vector<double>(meshCount, 0.0);
}

std::size_t SpatialMesh::getNumDivs() { return nDivs; }

void SpatialMesh::printMesh() {
  for (std::size_t i = 0; i < mesh.size(); ++i) {
    std::cout << i << " = (";
    /*
    std::vector<std::size_t> meshPoints = vectorIdxToMeshPoint(i);

    for (std::size_t j = 0; j < meshPoints.size(); ++j) {
      auto point = meshPoints.at(j);
      double coordPoint = point * static_cast<double>(bounds.at(j)) / nDivs;
      std::cout << std::fixed << std::setprecision(2) << coordPoint;
      if (j != meshPoints.size() - 1) {
        std::cout << ", ";
      }
    }
    std::cout << ") = " << getFValAtIdx(i) << " ";
    */

    /*
    if ((i + 1) % nDivs == 0) {
      std::cout << "\n";

    }
    */
  }
  std::cout << "\n";
}

void SpatialMesh::setFValAtMeshPoint(std::vector<std::size_t> meshPoint,
                                     double val) {
  auto idx = meshPointToVectorIdx(meshPoint);

  mesh.at(idx) = val;
}

double SpatialMesh::getFValAtMeshPoint(std::vector<std::size_t> meshPoint) {
  auto idx = meshPointToVectorIdx(meshPoint);
  return mesh.at(idx);
}

void SpatialMesh::setFValAtIdx(std::size_t idx, double val) {
  mesh.at(idx) = val;
}

double SpatialMesh::getFValAtIdx(std::size_t idx) { return mesh.at(idx); }

std::size_t
SpatialMesh::meshPointToVectorIdx(std::vector<std::size_t> meshPoint) {
  if (meshPoint.size() == dimension) {
    std::size_t idx = 0;
    for (std::size_t i = 0; i < dimension; ++i) {
      idx += meshPoint.at(i) * static_cast<int>(std::pow(nDivs, i));
    }

    return idx;
  } else {
    throw std::runtime_error(
        std::format("Input meshPoints dimension is inconsistent with mesh "
                    "dimension (Dimension: {}, Input: {})",
                    dimension, meshPoint.size()));
  }
}

std::vector<std::size_t>
SpatialMesh::vectorIdxToMeshPoint(std::size_t vectorIdx) {
  std::vector<std::size_t> meshPoints;
  for (std::size_t i = 0; i < dimension; ++i) {
    meshPoints.push_back(vectorIdx % nDivs);
    vectorIdx /= nDivs;
  }

  return meshPoints;
}

HeatEquationProblem::HeatEquationProblem(
    double diffusionCoefficient, std::shared_ptr<SpatialMesh> spatialDomain,
    Function_handle _source)
    : diffusionCoefficient(diffusionCoefficient), domain(spatialDomain),
      source(_source) {
  // Construct the tridiagonal matrix
}

HeatEquationProblem::~HeatEquationProblem() {}

double HeatEquationProblem::getDifussionCoefficient() {
  return diffusionCoefficient;
}

double HeatEquationProblem::evaluateSource(std::size_t i, std::size_t j) {
  return source(std::vector<std::size_t>({i, j}), 0);
}

std::shared_ptr<SpatialMesh> HeatEquationProblem::getDomainPtr() {
  return domain;
}

std::vector<double> *SpatialMesh::getDomainAsVectorPtr() { return &mesh; }
} // namespace PDE
