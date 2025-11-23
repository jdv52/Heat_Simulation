#pragma once

#include "PDE.hpp"

namespace HeatSim {

class SimulationSnapshot {
public:
  SimulationSnapshot(bool isRunning, long nIters, double simTimeMS,
                     double actualUpdatePeriodMS,
                     double algorithmCompletionTimeMS, PDE::SpatialMesh mesh)
      :

        _isRunning{isRunning}, _nIters{nIters}, _simTimeMS{simTimeMS},
        _actualUpdatePeriodMS{actualUpdatePeriodMS},
        _algorithmCompletionTimeMS{algorithmCompletionTimeMS}, _mesh{mesh} {}
  ~SimulationSnapshot() {}

  bool isRunning() const { return _isRunning; }
  long nIters() const { return _nIters; }
  double simTimeMS() const { return _simTimeMS; }
  double actualUpdatePeriodMS() const { return _actualUpdatePeriodMS; }
  double algorithmCompletionTimeMS() const {
    return _algorithmCompletionTimeMS;
  }
  PDE::SpatialMesh &mesh() { return _mesh; }

private:
  bool _isRunning;
  long _nIters;
  double _simTimeMS;
  double _actualUpdatePeriodMS;
  double _algorithmCompletionTimeMS;
  PDE::SpatialMesh _mesh;
  // residual
  // converge warnings
  // spatial mesh
};

} // namespace HeatSim
