#pragma once
#include "../Core/header/TimeSystem.hpp"
#include "imgui.h"

class DateHUD
{
public:
    void Draw(TimeDataModel& time);

private:
    ImVec2 m_position;
    ImVec2 m_windowSize = ImVec2(200.f, 100.f);
    ImGuiWindowFlags m_flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoBackground; // fajnie wygląda jak overlay

};