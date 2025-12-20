#pragma once
#include "imgui.h"
#include "../Research/ResearchManager.hpp"
#include <string>

using std::string;

class ResearchHUD
{
public:
    ResearchHUD() = default;
    void Draw(ResearchManager& manager);
    void DrawTechTree(ResearchManager& manager);
    void RegisterListeners(ResearchManager& manager);
private:
    void DrawTechNode(ResearchManager& manager, const Technology& tech);
    void DrawResearchCompletedPopup(); 
private:
    bool m_showPopup = false;
    string m_completedTechName;

    ImGuiWindowFlags m_flags =
        ImGuiWindowFlags_NoCollapse;
};
