#include "graphics/SimulationToolbar.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include <format>

using namespace HeatSim;

SimulationToolbar::SimulationToolbar() {}

SimulationToolbar::~SimulationToolbar() {}

void SimulationToolbar::display(bool *, SimulationSnapshot &snapshot) {
  if (ImGui::BeginMainMenuBar()) {

    ImGui::Text("Number of Simulation Iterations: %-10ld", snapshot.nIters());
    ImGui::Separator();
    ImGui::Text("Simulation Time (ms): %-10.2f", snapshot.simTimeMS());
    ImGui::Separator();
    ImGui::Text("Actual Simulation Period: %-10.2f",
                snapshot.actualUpdatePeriodMS());
    ImGui::Separator();
    ImGui::Text("Average Update Frequency: %-10.2f",
                static_cast<float>(snapshot.nIters()) / snapshot.simTimeMS() *
                    1000.0);
    ImGui::Separator();
    ImGui::Text("Algorithm completion time: %-10.2f",
                snapshot.algorithmCompletionTimeMS());

    ImGui::EndMainMenuBar();
  }
}
