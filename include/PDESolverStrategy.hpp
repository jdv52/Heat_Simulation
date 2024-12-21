#ifndef PDESolverStrategy_H
#define PDESolverStrategy_H

class PDE;

class PDESolverStrategy {
    public:
    
        PDESolverStrategy();
        ~PDESolverStrategy();

        void solve(PDE pde);
};

#endif