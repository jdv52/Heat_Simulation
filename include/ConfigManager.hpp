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
  void loadPlugins(std::filesystem::path path);

private:
  SimulationConfig cfg;
};

} // namespace HeatSim
