#pragma once
#include "imgui.h"

struct UIVisibility
{
    bool showDate = true;
    bool showResearch = true;
    bool showTechTree = true;
    bool showResources = true;
};

class TopBarHUD
{
public:
    void Draw(UIVisibility& ui);
};
