#include "graphics/SimulationWindow.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <csignal>
#include <iostream>

using namespace HeatSim;

SimulationWindow::SimulationWindow(
    std::shared_ptr<Simulation::SimRenderPipeline> sim_input,
    std::shared_ptr<Simulation::SimCommandPipeline> cmd_output)
    : input_pipe(sim_input), cmd_pipe(cmd_output) {}

SimulationWindow::~SimulationWindow() {}

bool SimulationWindow::start() {
  if (!driver_thread.joinable()) {
    driver_thread = std::jthread{[this](std::stop_token st) { this->run(st); }};
    return true;
  } else {
    return false;
    std::cout << "Simulation window is already open!\n";
  }
}

void SimulationWindow::processInputEvents(sf::Event event) {
  if (event.is<sf::Event::Closed>()) {
    window.close();
  } else if (event.is<sf::Event::MouseButtonPressed>()) {
    mouseDown = true;
    switch (event.getIf<sf::Event::MouseButtonPressed>()->button) {
    case sf::Mouse::Button::Left:
      drawScaleMultiplier = 1.0;
      break;
    case sf::Mouse::Button::Right:
      drawScaleMultiplier = -1.0;
      break;
    default:
      break;
    }
  } else if (event.is<sf::Event::MouseButtonReleased>()) {
    mouseDown = false;
  } else if (event.is<sf::Event::KeyPressed>()) {
    if (event.getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::K) {
      cmd_pipe->push(new SimToggleCommand{});
    }
  }
}

void SimulationWindow::updateGraphics(
    std::shared_ptr<SimulationSnapshot> state) {
  ImGui::SFML::Update(window, deltaClock.restart());

  window.clear();

  // Update all SFML widgets here

  if (state.get() != nullptr) {

    bool p_open;

    toolbar.display(&p_open, *state);
    controlPanel.display(&p_open, *state);

    heatMap->draw(state->mesh());
  }

  ImGui::ShowDemoWindow();
  ImGui::SFML::Render(window);
  window.display();
}

void SimulationWindow::run(std::stop_token st) {
  mouseDown = false;
  drawScaleMultiplier = 1.0;
  window = sf::RenderWindow{sf::VideoMode({1960, 1280}), "Heat Simulation",
                            sf::Style::Close};
  window.setFramerateLimit(60);
  if (ImGui::SFML::Init(window)) {

    deltaClock = sf::Clock{};

    std::cout << "Started simulation window\n";

    heatMap = std::make_unique<HeatMap>(
        1280 / 20, 20, window,
        HeatMapGradient{std::vector<sf::Color>(
            {sf::Color::Blue, sf::Color::Cyan, sf::Color::Green,
             sf::Color::Yellow, sf::Color::Red, sf::Color::White})});

    while (!st.stop_requested() && window.isOpen()) {
      auto state = input_pipe->read();

      while (const auto event = window.pollEvent()) {
        ImGui::SFML::ProcessEvent(window, *event);
        processInputEvents(*event);
      }

      if (mouseDown) {
        std::cout << "HIII\n";

        // get mouse position and enqueue a draw command
      }

      updateGraphics(state);
    }
  }

  raise(SIGINT);

  std::cout << "Simulation window terminating.\n";
}
