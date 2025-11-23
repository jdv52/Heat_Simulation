#ifndef PDE_H
#define PDE_H

#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <vector>

namespace PDE {

typedef std::function<double(std::vector<std::size_t>, double)> Function_handle;

// typedef double(*Function_handle)(std::vector<double> x, double t);

typedef double (*PDE_Function_handle)(double x, double y, double t, double u,
                                      double dudt, double dudx);

class SpatialMesh {
public:
  SpatialMesh() {}

  /**
   * @brief Creates a N-dimensional discretized rectangular mesh given bounds
   * and number of divisions
   *
   * The created n-dimensional mesh spans between 0 and the upper bounds
   * specified by the bounds vector. All dimensions are assumed to have the same
   * number of divisions and uniform spacing.
   *
   * @param bounds the upper bounds of the mesh
   * @param nDivs the number of divisions along each dimension
   */
  SpatialMesh(std::vector<double> bounds, std::size_t nDivs);

  std::size_t getNumDivs();

  /**
   * @brief Prints the mesh to the terminal as a 2D grid of points (for
   * debugging purposes)
   *
   */
  void printMesh();

  /**
   * @brief A utility function to assign a value to a specified meshpoint
   *
   * The input meshpoint should be given as a vector of integers where the 1st
   * element specifies the nth mesh point along the 1st dimension, etc.
   *
   * @param meshPoint a vector specifying the coordinates of the mesh point to
   * set
   * @param val the value to assign to the meshPoin
   */
  void setFValAtMeshPoint(std::vector<std::size_t> meshPoint, double val);

  /**
   * @brief A utility function to retrieve the assigned value at a specified
   * meshpoint
   *
   * @param meshPoint a vector specifying the coordinates of the mesh point to
   * read
   * @return the corresponding double value at meshPoint
   */
  double getFValAtMeshPoint(std::vector<std::size_t> meshPoint);

  /**
   * @brief A utility function to assign a value to a specified index
   *
   * @param idx an integer corresponding to a meshpoint in the mesh
   * @param val the value to assign to the idx
   */
  void setFValAtIdx(std::size_t idx, double val);

  /**
   * @brief A utility function to retrieve the assigned value at a specified
   * meshpoint
   *
   * @param idx an integer corresponding to a meshpoint in the mesh
   * @return the corresponding double value at idx
   */
  double getFValAtIdx(std::size_t idx);

  std::vector<double> *getDomainAsVectorPtr();

private:
  std::size_t nDivs;
  std::size_t dimension;
  std::vector<double> bounds;
  std::vector<double> mesh;

  /**
   * @brief A utility function to convert a n-d mesh point to a 1-D vector idx
   *
   * The mesh is stored internally as a vector for more efficient storage and
   * faster lookup time, so this utility function is provided to convert
   * from a meshpoint coordinate to the vector index
   *
   * @param meshPoint the mesh point to convert
   * @return a vector index as an integer
   */
  std::size_t meshPointToVectorIdx(std::vector<std::size_t> meshPoint);

  /**
   * @brief A utility function to convert a vector idx to a n-D meshpoint
   * coordinates
   *
   * @param vectorIdx the vector index to convert as an integer
   * @return the coordinates of the meshpoint corresponding to vectorIdx as a
   * vector of ints
   */
  std::vector<std::size_t> vectorIdxToMeshPoint(std::size_t vectorIdx);
};

class BoundaryCondition {
public:
  /**
   * @brief Construct a new Boundary Condition object
   *
   * Represents boundary conditions in the form:
   *
   * alpha(t, x_n) * u(t, x_n) + beta * ux(t, x_n) = G(t, x_n)
   *
   * So for example, if we wanted to set Dirichlet boundary conditions,
   * u(0, t) = 0, we would have alpha = 1, beta = 0, G = 0. These functions
   * are also time-dependent if needed for the problem.
   *
   * @param x the coordinate(s) on which the boundary condition is imposed
   * @param alpha the alpha function as specified above
   * @param beta the beta function as specified above
   * @param G the G function as specified above
   */
  BoundaryCondition(double x, Function_handle alpha, Function_handle beta,
                    Function_handle G);

private:
  double x;
  Function_handle alpha;
  Function_handle beta;
  Function_handle G;
};

class HeatEquationProblem {
public:
  HeatEquationProblem(double diffusionCoefficient,
                      std::shared_ptr<SpatialMesh> spatialDomain,
                      Function_handle source);

  ~HeatEquationProblem();

  double getDifussionCoefficient();
  double evaluateSource(std::size_t i, std::size_t j);
  std::shared_ptr<SpatialMesh> getDomainPtr();

private:
  double diffusionCoefficient;
  std::shared_ptr<SpatialMesh> domain;
  Function_handle source;
};
} // namespace PDE

#endif
