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
    bool IsTechTreeVisible() const { return m_visibleTechTree; }
    void SetVisible(bool v) { m_visible = v; }
    void SetVisibleTechTree(bool v) { m_visibleTechTree = v; }

private:
    void DrawTechNode(ResearchManager &manager, const Technology &tech);
    void DrawResearchCompletedPopup();

private:
    bool m_showPopup = false;
    bool m_visible = true;
    bool m_visibleTechTree = true;
    string m_completedTechName;

    ImGuiWindowFlags m_flags =
        ImGuiWindowFlags_NoCollapse;
};
