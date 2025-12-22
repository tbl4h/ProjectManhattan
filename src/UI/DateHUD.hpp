#pragma once
#include "../Core/header/TimeSystem.hpp"
#include "IHUD.hpp"
#include "imgui.h"

class DateHUD : public IHUD
{
public:
    void Draw(TimeDataModel &time);
    bool IsVisible() const override { return m_visible; }
    void SetVisible(bool v) override { m_visible = v; }

private:
    ImVec2 m_position;
    ImVec2 m_windowSize = ImVec2(200.f, 100.f);
    ImGuiWindowFlags m_flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoBackground; // fajnie wygląda jak overlay
    bool m_visible = true;
};