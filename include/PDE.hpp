#ifndef PDE_H
#define PDE_H

#include <vector>
#include <functional>
#include <memory>

namespace PDE {

    typedef std::function<float(std::vector<int>, float)> Function_handle;

    // typedef float(*Function_handle)(std::vector<float> x, float t);

    typedef float(*PDE_Function_handle)(float x, float y, float t, float u, float dudt, float dudx);

    class SpatialMesh
    {
        public:
            /**
             * @brief Creates a N-dimensional discretized rectangular mesh given bounds and number of divisions
             * 
             * The created n-dimensional mesh spans between 0 and the upper bounds specified by the bounds vector.
             * All dimensions are assumed to have the same number of divisions and uniform spacing.
             * 
             * @param bounds the upper bounds of the mesh
             * @param nDivs the number of divisions along each dimension
             */
            SpatialMesh(std::vector<float> bounds, int nDivs);

            int getNumDivs();
            
            /**
             * @brief Prints the mesh to the terminal as a 2D grid of points (for debugging purposes)
             * 
             */
            void printMesh();

            /**
             * @brief A utility function to assign a value to a specified meshpoint
             * 
             * The input meshpoint should be given as a vector of integers where the 1st element
             * specifies the nth mesh point along the 1st dimension, etc.
             * 
             * @param meshPoint a vector specifying the coordinates of the mesh point to set
             * @param val the value to assign to the meshPoin
             */
            void setFValAtMeshPoint(std::vector<int> meshPoint, float val);

            /**
             * @brief A utility function to retrieve the assigned value at a specified meshpoint
             * 
             * @param meshPoint a vector specifying the coordinates of the mesh point to read
             * @return the corresponding float value at meshPoint
             */
            float getFValAtMeshPoint(std::vector<int> meshPoint);

            /**
             * @brief A utility function to assign a value to a specified index
             * 
             * @param idx an integer corresponding to a meshpoint in the mesh
             * @param val the value to assign to the idx
             */
            void setFValAtIdx(int idx, float val);

            /**
             * @brief A utility function to retrieve the assigned value at a specified meshpoint
             * 
             * @param idx an integer corresponding to a meshpoint in the mesh
             * @return the corresponding float value at idx
             */
            float getFValAtIdx(int idx);

            std::vector<float>* getDomainAsVectorPtr();

        private:
            int nDivs;
            int dimension;
            std::vector<float> bounds;
            std::vector<float> mesh;

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
            int meshPointToVectorIdx(std::vector<int> meshPoint);

            /**
             * @brief A utility function to convert a vector idx to a n-D meshpoint coordinates
             * 
             * @param vectorIdx the vector index to convert as an integer
             * @return the coordinates of the meshpoint corresponding to vectorIdx as a vector of ints
             */
            std::vector<int> vectorIdxToMeshPoint(int vectorIdx);
    };

    class BoundaryCondition
    {
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
            BoundaryCondition(float x, Function_handle alpha, Function_handle beta, Function_handle G);

        private:

            float x;
            Function_handle alpha;
            Function_handle beta;
            Function_handle G;
    };

    class HeatEquationProblem
    {
        public:
            HeatEquationProblem(
                float diffusionCoefficient,
                std::shared_ptr<SpatialMesh> spatialDomain,
                Function_handle source
            );

            ~HeatEquationProblem();

            float getDifussionCoefficient();
            float evaluateSource(int i, int j);
            std::shared_ptr<SpatialMesh> getDomainPtr();

        private:
            float diffusionCoefficient;
            std::shared_ptr<SpatialMesh> domain;
            Function_handle source;
    };
}

#endif