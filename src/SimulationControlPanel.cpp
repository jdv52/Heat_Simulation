#include "graphics/SimulationControlPanel.hpp"

using namespace HeatSim;

static void HelpMarker(const char *desc) {
  ImGui::TextDisabled("(?)");
  if (ImGui::BeginItemTooltip()) {
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
}

SimulationControlPanel::SimulationControlPanel() { window_flags = 0; }

SimulationControlPanel::~SimulationControlPanel() {}

void SimulationControlPanel::display(bool *p_open, SimulationSnapshot &) {
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

  ImVec2 sz = ImVec2(-FLT_MIN, 0.0f);

  ImGui::Button("Load Default Configuration", sz);

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
    ImGui::SeparatorText("General");

    static double dt = 10.0;
    ImGui::InputDouble("Delta Time (ms)", &dt, 0.01f, 1.0f, "%.8f");
    ImGui::SameLine();
    HelpMarker("The desired period between simulation iterations.");

    static int vec2i0[2] = {10, 10};
    ImGui::InputInt2("Grid Divisions", vec2i0);
    ImGui::SameLine();
    HelpMarker("The number of mesh points in the X and Y axes, respectively.");

    static int vec2i1[2] = {-1, 1};
    ImGui::InputInt2("X Bounds", vec2i1);
    ImGui::SameLine();
    HelpMarker("The domain on the X-axis.");

    static int vec2i2[2] = {-1, 1};
    ImGui::InputInt2("Y Bounds", vec2i2);
    ImGui::SameLine();
    HelpMarker("The domain on the Y-axis.");

    static int item_selected_idx = 0;
    const char *items[] = {"Forward Difference",
                           "BBBB",
                           "CCCC",
                           "DDDD",
                           "EEEE",
                           "FFFF",
                           "GGGG",
                           "HHHH",
                           "IIII",
                           "JJJJ",
                           "KKKK",
                           "LLLLLLL",
                           "MMMM",
                           "OOOOOOO"};
    const char *combo_preview_value = items[item_selected_idx];
    if (ImGui::BeginCombo("Solver method", combo_preview_value, 0)) {
      for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
        const bool is_selected = (item_selected_idx == n);
        if (ImGui::Selectable(items[n], is_selected))
          item_selected_idx = n;

        // Set the initial focus when opening the combo (scrolling + keyboard
        // navigation focus)
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
    ImGui::SameLine();
    HelpMarker("The PDE solver method to use.");

    ImGui::SeparatorText("Model");

    static double sigma = 10.0;
    ImGui::InputDouble("Diffusion Coefficient", &sigma, 0.01f, 1.0f, "%.8f");
    ImGui::SameLine();
    HelpMarker("A factor determining the rate at which heat spreads "
               "throughout the material.");

    static int bc_selected_idx = 0;
    const char *bcs[] = {"Dirichlet", "Neumann", "Robin", "Custom"};

    const char *combo_preview_bc_value = bcs[bc_selected_idx];
    if (ImGui::BeginCombo("Solver method", combo_preview_bc_value, 0)) {
      for (int n = 0; n < IM_ARRAYSIZE(bcs); n++) {
        const bool is_selected = (bc_selected_idx == n);
        if (ImGui::Selectable(bcs[n], is_selected))
          bc_selected_idx = n;

        // Set the initial focus when opening the combo (scrolling + keyboard
        // navigation focus)
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
    ImGui::SameLine();
    HelpMarker("The PDE solver method to use.");

    ImGui::SeparatorText("Source Function");

    static int brush_type_selected_idx = 0;
    const char *brush_types[] = {"Gaussian"};

    const char *brush_type_preview_bc_value =
        brush_types[brush_type_selected_idx];
    if (ImGui::BeginCombo("Brush Type", brush_type_preview_bc_value, 0)) {
      for (int n = 0; n < IM_ARRAYSIZE(brush_types); n++) {
        const bool is_selected = (brush_type_selected_idx == n);
        if (ImGui::Selectable(brush_types[n], is_selected))
          brush_type_selected_idx = n;

        // Set the initial focus when opening the combo (scrolling + keyboard
        // navigation focus)
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
    ImGui::SameLine();
    HelpMarker("The shape of the heat brush.");

    static double brush_strength = 10.0;
    ImGui::InputDouble("Strength", &brush_strength, 0.01f, 1.0f, "%.8f");
    ImGui::SameLine();
    HelpMarker("The strength of the heat brush.");

    static double brush_size = 10.0;
    ImGui::InputDouble("Size", &brush_size, 0.01f, 1.0f, "%.8f");
    ImGui::SameLine();
    HelpMarker("The size of the heat brush.");
  }

  if (ImGui::CollapsingHeader("Simulation Visuals")) {
    // Temperature gradient
    // Visuals frame rate
    // Cap frame rate
  }

  ImGui::End();
}
