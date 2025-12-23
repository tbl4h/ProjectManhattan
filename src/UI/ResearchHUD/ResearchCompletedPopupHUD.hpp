#pragma once
#include "imgui.h"
#include <string>

class ResearchCompletedPopupHUD
{
public:
    void Show(const std::string& techName);
    void Draw();

private:
    bool m_showPopup = false;
    std::string m_completedTechName;
};
