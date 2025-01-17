#ifndef PDESOLVER_H
#define PDESOLVER_H

#include "PDE.hpp"



class HeatEquationSolver {
    public:
    
        HeatEquationSolver(float dt);
        ~HeatEquationSolver();

        void setTimeStep(float dt);

        virtual void solve(PDE::HeatEquationProblem& heatEq) = 0;

    protected:
        float timeStep;
};

class ForwardDifference : public HeatEquationSolver {
    public:
        ForwardDifference(float dt);
        ~ForwardDifference();

        void solve(PDE::HeatEquationProblem& heatEq) override;
};


#endif