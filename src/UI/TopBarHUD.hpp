#pragma once
#include "imgui.h"
#include "UIVisibility.hpp"

/*
struct UIVisibility
{
    bool showDate = true;
    bool showResearch = true;
    bool showTechTree = true;
    bool showResources = true;
};
*/
// struct UIVisibility
// {
//     bool showResearch = true;
//     bool lastShowResearch = true;
// };

// template<typename HUD>
// void SyncVisibility(bool& uiFlag, bool& lastFlag, HUD& hud)
// {
//     if (uiFlag != lastFlag)
//     {
//         hud.SetVisible(uiFlag);
//         lastFlag = uiFlag;
//     }

//     uiFlag = hud.IsVisible();
// }

class TopBarHUD
{
public:
    void Draw(UIVisibility& ui);
};
