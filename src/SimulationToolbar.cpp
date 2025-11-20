#include "graphics/SimulationToolbar.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

using namespace HeatSim;

SimulationToolbar::SimulationToolbar() {}

SimulationToolbar::~SimulationToolbar() {}

void SimulationToolbar::display(bool *p_open,
                                Simulation::SimulationState &state) {
  if (ImGui::BeginMainMenuBar()) {

    ImGui::EndMainMenuBar();
  }
}
