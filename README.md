# HEAT SIMULATION

This program is a very simple simulation of the heat equation in a 2D spatial domain. The simulation uses a time-marching method
based on a few well-known algorithms for numerically solving PDEs. Namely:
- Finite Difference Method
- Crank-Nicholson
- Finite Element Methods
- Fourier Spectral Methods

Though the application is only for simulating the heat equation, the PDE solver was (hopefully) implemented in a way that should
make it usable for any generic PDE. Using an object-oriented approach, I developed a PDE solver which encapsulates the PDE as
a C++ class, which can then be plugged into any of the implemented solvers to achieve a numerical solution.