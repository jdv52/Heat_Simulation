#ifndef PDESOLVER_H
#define PDESOLVER_H

#include "PDE.hpp"



class HeatEquationSolver {
    public:
    
        HeatEquationSolver(double dt);
        ~HeatEquationSolver();

        void setTimeStep(double dt);

        virtual void solve(PDE::HeatEquationProblem& heatEq) = 0;

    protected:
        double timeStep;
};

class ForwardDifference : public HeatEquationSolver {
    public:
        ForwardDifference(double dt);
        ~ForwardDifference();

        void solve(PDE::HeatEquationProblem& heatEq) override;
};


#endif