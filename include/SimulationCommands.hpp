#pragma once

enum class CommandType { simStart, simStop };

namespace HeatSim {

class SimulationManager;

class SimCommandBase {

public:
  SimCommandBase() {}

  virtual ~SimCommandBase() {}

  virtual void exec(SimulationManager &) = 0;

protected:
  CommandType type;
};

class SimToggleCommand : public SimCommandBase {
public:
  SimToggleCommand();

  ~SimToggleCommand();

  virtual void exec(SimulationManager &sim) override;
};

class SimResumeCommand : public SimCommandBase {
public:
  SimResumeCommand();

  ~SimResumeCommand();

  virtual void exec(SimulationManager &sim) override;
};

} // namespace HeatSim
