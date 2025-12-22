#pragma once
#include <chrono>
#include "imgui.h"
#include "../Resources/ResourcesManager.hpp"
#include "../Research/ResearchManager.hpp"
#include "IHUD.hpp"

class ResourcesHUD : public IHUD
{
public:
    ResourcesHUD();

    void Draw(ResourcesManager &manager);
    void OnResearchMissingResources(
        const ResourceMissing &missing);
    bool IsVisible() const override { return m_visible; }
    void SetVisible(bool v) override { m_visible = v; }

private:
    void DrawMoney(ResourcesManager &manager);
    void DrawMaterials(ResourcesManager &manager);
    void DrawPersonnel(ResourcesManager &manager);
    void DrawFacilityStats(ResourcesManager &manager);

    static void HighlightIf(bool condition);
    static void EndHighlightIf(bool condition);
    void UpdateHighlightTimer();

private:
    ResourceMissing m_misingResources;

    std::chrono::steady_clock::time_point m_highlightUntil;
    bool m_visible = true;

    ImGuiWindowFlags m_flags =
        ImGuiWindowFlags_NoCollapse;
};
