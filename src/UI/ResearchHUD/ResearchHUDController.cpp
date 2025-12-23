#include "ResearchHUDController.hpp"

ResearchHUDController::ResearchHUDController(
    ResearchManager& manager,
    ResearchCompletedPopupHUD& popupHUD)
    : m_manager(manager)
    , m_popupHUD(popupHUD)
{
    // 🔴 Controller słucha Modelu
    m_manager.addResearchCompletedListener(
        [this](const Technology& tech)
        {
            m_popupHUD.Show(tech.m_name);
        });
}

void ResearchHUDController::StartResearch(const std::string& techId)
{
    m_manager.startResearch(techId);
}
