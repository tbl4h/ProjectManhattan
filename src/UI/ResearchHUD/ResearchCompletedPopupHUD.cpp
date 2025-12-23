#include "ResearchCompletedPopupHUD.hpp"

void ResearchCompletedPopupHUD::Show(const std::string& techName)
{
    m_completedTechName = techName;
    m_showPopup = true;
}

void ResearchCompletedPopupHUD::Draw()
{
    if (m_showPopup)
    {
        ImGui::OpenPopup("Research Completed");
        m_showPopup = false; // tylko raz
    }

    if (ImGui::BeginPopupModal(
            "Research Completed",
            nullptr,
            ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Research completed successfully!");
        ImGui::Separator();

        ImGui::TextColored(
            ImVec4(0.3f, 1.f, 0.3f, 1.f),
            "%s",
            m_completedTechName.c_str());

        ImGui::Spacing();

        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
