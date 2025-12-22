#pragma once
#include "IHUD.hpp"
#include "imgui.h"
#include "ResearchHUDController.hpp"

class ResearchListHUD : public IHUD
{
public:
    explicit ResearchListHUD(ResearchHUDController& controller)
        : m_controller(controller) {}

    void Draw();

    bool IsVisible() const override { return m_visible; }
    void SetVisible(bool v) override { m_visible = v; }

private:
    ResearchHUDController& m_controller;
    bool m_visible = true;

    ImGuiWindowFlags m_flags =
        ImGuiWindowFlags_NoCollapse;
};
