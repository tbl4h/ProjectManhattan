#pragma once
#include "imgui.h"
#include "../Resources/ResourcesManager.hpp"

class ResourcesHUD
{
public:
    ResourcesHUD() = default;

    void Draw(ResourcesManager& manager);

private:
    void DrawMoney(ResourcesManager& manager);
    void DrawMaterials(ResourcesManager& manager);
    void DrawPersonnel(ResourcesManager& manager);
    void DrawFacilityStats(ResourcesManager& manager);

private:
    ImGuiWindowFlags m_flags =
        ImGuiWindowFlags_NoCollapse;
};
