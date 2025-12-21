#pragma once
#include "imgui.h"
#include "../Research/ResearchManager.hpp"
#include <string>

using std::string;

class ResearchHUD
{
public:
    ResearchHUD() = default;
    void Draw(ResearchManager &manager);
    void DrawTechTree(ResearchManager &manager);
    void RegisterListeners(ResearchManager &manager);
    bool IsVisible() const { return m_visible; }
    void SetVisible(bool v) { m_visible = v; }

private:
    void DrawTechNode(ResearchManager &manager, const Technology &tech);
    void DrawResearchCompletedPopup();

private:
    bool m_showPopup = false;
    bool m_visible = true;
    string m_completedTechName;

    ImGuiWindowFlags m_flags =
        ImGuiWindowFlags_NoCollapse;
};
