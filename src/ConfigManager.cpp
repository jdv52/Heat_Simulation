#include "ConfigManager.hpp"
#include <dlfcn.h>
#include <format>
#include <fstream>
#include <inicpp.h>
#include <iostream>

using namespace HeatSim;

namespace ini {
template <typename T, typename U> struct Convert<std::pair<T, U>> {
  void decode(const std::string &value, std::pair<T, U> &result) {
    T firstDecoded;
    U secondDecoded;

    auto n = value.find(",");

    Convert<T> convFirst;
    std::string firstStr = value.substr(0, n);
    convFirst.decode(firstStr, firstDecoded);

    Convert<U> convSecond;
    std::string secondStr = value.substr(n + 1);
    convSecond.decode(secondStr, secondDecoded);

    result = std::pair<T, U>(firstDecoded, secondDecoded);
  }

  void encode(const std::pair<T, U> &value, std::string &result) {
    // variable to store the encoded element value
    std::string encodedFirst;
    Convert<T> convFirst;
    convFirst.encode(std::get<0>(value), encodedFirst);

    std::string encodedSecond;
    Convert<T> convSecond;
    convSecond.encode(std::get<1>(value), encodedSecond);

    result = std::format("{}, {}", convFirst, convSecond);
  }
};

template <typename T> struct Convert<std::vector<T>> {
  /** Decodes a std::vector from a string. */
  void decode(const std::string &value, std::vector<T> &result) {
    result.clear();

    // variable to store the decoded value of each element
    T decoded;
    // maintain a start and end pos within the string
    size_t startPos = 0;
    size_t endPos = 0;
    size_t cnt;

    while (endPos != std::string::npos) {
      if (endPos != 0)
        startPos = endPos + 1;
      // search for the next comma as separator
      endPos = value.find(',', startPos);

      // if no comma was found use the rest of the string
      // as input
      if (endPos == std::string::npos)
        cnt = value.size() - startPos;
      else
        cnt = endPos - startPos;

      std::string tmp = value.substr(startPos, cnt);
      // use the conversion functor for the type contained in
      // the vector, so the vector can use any type that
      // is compatible with inifile-cpp
      Convert<T> conv;
      conv.decode(tmp, decoded);
      result.push_back(decoded);
    }
  }

  /** Encodes a std::vector to a string. */
  void encode(const std::vector<T> &value, std::string &result) {
    // variable to store the encoded element value
    std::string encoded;
    // string stream to build the result stream
    std::stringstream ss;
    for (size_t i = 0; i < value.size(); ++i) {
      // use the conversion functor for the type contained in
      // the vector, so the vector can use any type that
      // is compatible with inifile-cp
      Convert<T> conv;
      conv.encode(value[i], encoded);
      ss << encoded;

      // if this is not the last element add a comma as separator
      if (i != value.size() - 1)
        ss << ',';
    }
    // store the created string in the result
    result = ss.str();
  }
};
} // namespace ini

ConfigManager::ConfigManager() {}

ConfigManager::~ConfigManager() {}

void ConfigManager::loadConfigFile(std::filesystem::path path) {
  std::ifstream file(path);

  ini::IniFile cfgIni;
  cfgIni.decode(file);

  cfg.setDt(cfgIni["SimulationConfiguration//General"]["dt"].as<double>());

  auto nDivs = cfgIni["SimulationConfiguration//General"]["nDivs"]
                   .as<std::vector<std::size_t>>();
  cfg.setNDivs(std::vector<std::size_t>(nDivs));

  auto xBounds = cfgIni["SimulationConfiguration//General"]["xBounds"]
                     .as<std::pair<double, double>>();
  auto yBounds = cfgIni["SimulationConfiguration//General"]["xBounds"]
                     .as<std::pair<double, double>>();
  cfg.setBounds(std::vector<std::pair<double, double>>({xBounds, yBounds}));

  cfg.setDiffusionCoefficient(
      cfgIni["SimulationConfiguration//Model"]["diffusionCoefficient"]
          .as<double>());

  std::filesystem::path pluginFolder =
      cfgIni["SimulationConfiguration//General"]["pluginFolder"]
          .as<std::string>();
  loadPlugins(pluginFolder);
}

void ConfigManager::loadPlugins(std::filesystem::path) {

  void *handle = dlopen("./plugins/libforward_difference.so", RTLD_LAZY);
  if (handle) {
    dlclose(handle);
  } else {
    fprintf(stderr, "dlerror: %s\n", dlerror());
  }
}

SimulationConfig ConfigManager::getConfig() { return cfg; }
