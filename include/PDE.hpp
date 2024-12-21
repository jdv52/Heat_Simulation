#ifndef PDE_H
#define PDE_H

#include "PDESolverStrategy.hpp"

class PDE
{
    public:
    
        PDE();
        ~PDE();

        void setSolutionMethod(PDESolverStrategy strategy);
        void solve();

    private:
        PDESolverStrategy strategy;
};

#endif