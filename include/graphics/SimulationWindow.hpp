#pragma once

#include "Simulation.hpp"
#include "SimulationControlPanel.hpp"
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
  SimulationWindow(std::shared_ptr<Simulation::SimRenderPipeline> sim_input);

  ~SimulationWindow();

  bool start();

private:
  std::jthread driver_thread;

  void run(std::stop_token st);
  void processInputEvents(sf::Event event);
  void updateGraphics(Simulation::SimulationState &state);

  std::shared_ptr<Simulation::SimRenderPipeline> input_pipe;

  sf::RenderWindow window;
  sf::Clock deltaClock;

  int drawScaleMultiplier;
  bool mouseDown;

  SimulationToolbar toolbar;
  SimulationControlPanel controlPanel;
};

} // namespace HeatSim
