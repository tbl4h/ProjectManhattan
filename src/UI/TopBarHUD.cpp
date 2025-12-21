#include "TopBarHUD.hpp"

void TopBarHUD::Draw(UIVisibility& ui)
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, 35));

    ImGui::Begin("TopBar",
                 nullptr,
                 ImGuiWindowFlags_NoDecoration |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoSavedSettings |
                 ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImGui::Text("Manhattan Project");
    ImGui::SameLine(200);

    ImGui::Checkbox("Date", &ui.showDate);
    ImGui::SameLine();
    ImGui::Checkbox("Research", &ui.showResearch);
    ImGui::SameLine();
    ImGui::Checkbox("Tech Tree", &ui.showTechTree);
    ImGui::SameLine();
    ImGui::Checkbox("Resources", &ui.showResources);

    ImGui::End();
}
