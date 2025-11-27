#pragma once

#include "SimulationConfig.hpp"
#include <filesystem>

namespace HeatSim {
class ConfigManager {

public:
  ConfigManager();
  ~ConfigManager();

  void loadConfigFile(std::filesystem::path path);
  SimulationConfig getConfig();

private:
  SimulationConfig cfg;
};

} // namespace HeatSim
