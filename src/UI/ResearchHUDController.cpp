#include "ResearchHUDController.hpp"
#include <string>

using std::string;

// akcje użytkownika
void ResearchHUDController::StartResearch(const std::string &techId)
{
    m_manager.startResearch(techId);
}