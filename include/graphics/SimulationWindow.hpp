#pragma once

#include "HeatMap.hpp"
#include "SimulationControlPanel.hpp"
#include "SimulationManager.hpp"
#include "SimulationSnapshot.hpp"
#include "SimulationToolbar.hpp"
#include "graphics/SimulationControlPanel.hpp"
#include "graphics/SimulationToolbar.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <thread>

namespace HeatSim {

class SimulationWindow {
public:
  SimulationWindow(
      std::shared_ptr<SimulationManager::SimRenderPipeline> sim_input,
      std::shared_ptr<SimulationManager::SimCommandPipeline> cmd_output);

  ~SimulationWindow();

  bool start();

private:
  std::jthread driver_thread;

  void run(std::stop_token st);
  void processInputEvents(sf::Event event);
  void updateGraphics(std::shared_ptr<SimulationSnapshot> state);

  std::shared_ptr<SimulationManager::SimRenderPipeline> input_pipe;
  std::shared_ptr<SimulationManager::SimCommandPipeline> cmd_pipe;

  sf::RenderWindow window;
  sf::Clock deltaClock;

  int drawScaleMultiplier;
  bool mouseDown;

  std::unique_ptr<HeatMap> heatMap;
  SimulationToolbar toolbar;
  SimulationControlPanel controlPanel;
};

} // namespace HeatSim
