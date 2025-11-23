#pragma once

enum class CommandType { simStart, simStop };

namespace HeatSim {

class Simulation;

class SimCommandBase {

public:
  SimCommandBase() {}

  virtual ~SimCommandBase() {}

  virtual void exec(Simulation &) = 0;

protected:
  CommandType type;
};

class SimToggleCommand : public SimCommandBase {
public:
  SimToggleCommand();

  ~SimToggleCommand();

  virtual void exec(Simulation &sim) override;
};

class SimResumeCommand : public SimCommandBase {
public:
  SimResumeCommand();

  ~SimResumeCommand();

  virtual void exec(Simulation &sim) override;
};

} // namespace HeatSim
