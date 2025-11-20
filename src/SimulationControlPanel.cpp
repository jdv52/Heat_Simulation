#include "graphics/SimulationControlPanel.hpp"

using namespace HeatSim;

SimulationControlPanel::SimulationControlPanel() { window_flags = 0; }

SimulationControlPanel::~SimulationControlPanel() {}

void SimulationControlPanel::display(bool *p_open,
                                     Simulation::SimulationState &state) {
  if (!ImGui::Begin("Simulation Control Panel", p_open, window_flags)) {

    ImGui::End();
    return;
  }

  // Main menu to
  // - Menu
  //    - Save configuration options
  //    - Load configuration options
  //    - Start/stop simulation
  //    - Quit simulation (modal)
  // - Tools
  //    - Profiler
  //      - Shows more detailed timing info with plots
  //    - Log window
  //    - Command console

  if (ImGui::CollapsingHeader("Help")) {
    ImGui::SeparatorText("ABOUT THIS DEMO:");
    ImGui::BulletText(
        "Sections below are demonstrating many aspects of the library.");
    ImGui::BulletText(
        "The \"Examples\" menu above leads to more demo contents.");
    ImGui::BulletText(
        "The \"Tools\" menu above gives access to: About Box, Style Editor,\n"
        "and Metrics/Debugger (general purpose Dear ImGui debugging tool).");

    ImGui::SeparatorText("PROGRAMMER GUIDE:");
    ImGui::BulletText(
        "See the ShowDemoWindow() code in imgui_demo.cpp. <- you are here!");
    ImGui::BulletText("See comments in imgui.cpp.");
    ImGui::BulletText("See example applications in the examples/ folder.");
    ImGui::BulletText("Read the FAQ at ");
    ImGui::SameLine(0, 0);
    ImGui::TextLinkOpenURL("https://www.dearimgui.com/faq/");
    ImGui::BulletText(
        "Set 'io.ConfigFlags |= NavEnableKeyboard' for keyboard controls.");
    ImGui::BulletText(
        "Set 'io.ConfigFlags |= NavEnableGamepad' for gamepad controls.");

    ImGui::SeparatorText("USER GUIDE:");
    ImGui::ShowUserGuide();
  }

  if (ImGui::CollapsingHeader("Simulation Configuration")) {
    // Simulation Configuration
    // - deltaTime
    // - gridCount
    // - solver method
    // - simulation domain
    // - Model Parameters
    //  - Diffusion coefficient
    //  - Boundary conditions
    // - Drawing Parameters
    //  - Type
    //  - Strength
    //  - Size
  }

  if (ImGui::CollapsingHeader("Simulation Visuals")) {
    // Temperature gradient
    // Visuals frame rate
    // Cap frame rate
  }

  ImGui::End();
}
