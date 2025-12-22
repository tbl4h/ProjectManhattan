#pragma once
#include "IHUD.hpp"

inline void SyncVisibility(
    bool& uiFlag,
    bool& lastFlag,
    IHUD& hud)
{
    // TopBar → HUD
    if (uiFlag != lastFlag)
    {
        hud.SetVisible(uiFlag);
        lastFlag = uiFlag;
    }

    // HUD (❌) → TopBar
    uiFlag = hud.IsVisible();
}
