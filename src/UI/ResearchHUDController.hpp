#pragma once
#include "../Research/ResearchManager.hpp"

class ResearchHUDController
{
public:
    explicit ResearchHUDController(ResearchManager &manager)
        : m_manager(manager) {}

    // akcje użytkownika
    void StartResearch(const std::string &techId);

    // dostęp do modelu (read-only)
    const ResearchManager &Model() const { return m_manager; }

private:
    ResearchManager &m_manager;
};
