#pragma once

#include "AbstractPDE.hpp"

namespace HeatSim {

class ParabolicPDE : public AbstractPDE {
public:
  ParabolicPDE(double diffusionCoefficient, forcing_function fn);
  ~ParabolicPDE();

  virtual double evalAt(std::vector<float> x) override;
  virtual bool checkIsStable() override;

private:
  double sigma;
  forcing_function fn;
};
} // namespace HeatSim
