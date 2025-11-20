#pragma once

enum class CommandType { simStart, simStop };

namespace HeatSim {

class SimCommandBase {

public:
  SimCommandBase();

  ~SimCommandBase();

  virtual void exec() = 0;

protected:
  CommandType type;
};

} // namespace HeatSim
