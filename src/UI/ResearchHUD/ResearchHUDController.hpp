#pragma once
#include "../../Research/ResearchManager.hpp"
#include "ResearchCompletedPopupHUD.hpp"

class ResearchHUDController
{
public:
    ResearchHUDController(
        ResearchManager& manager,
        ResearchCompletedPopupHUD& popupHUD);

    // akcje użytkownika
    void StartResearch(const std::string& techId);

    // dostęp do modelu (read-only)
    const ResearchManager& Model() const { return m_manager; }

private:
    ResearchManager& m_manager;
    ResearchCompletedPopupHUD& m_popupHUD;
};
