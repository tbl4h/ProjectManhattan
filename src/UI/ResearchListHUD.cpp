#include "ResearchListHUD.hpp"

void ResearchListHUD::Draw()
{
    if (!m_visible)
        return;

    if (!ImGui::Begin("Research", &m_visible, m_flags))
    {
        ImGui::End();
        return;
    }

    const auto& manager = m_controller.Model();

    ImGui::Text("Available Technologies");
    ImGui::Separator();

    for (const auto& [id, tech] : manager.getAllTechnologies())
    {
        bool locked = !manager.isAvailable(id) && !tech.isInProgress() && !tech.isCompleted();

        if (locked)
            ImGui::BeginDisabled();

        if (ImGui::Button(tech.m_name.c_str(), ImVec2(-1, 0)))
        {
            m_controller.StartResearch(id);
        }

        if (locked)
            ImGui::EndDisabled();

        if (tech.isInProgress())
        {
            float p = manager.getProgress(id);
            ImGui::ProgressBar(p, ImVec2(-1, 0));
        }

        if (tech.isCompleted())
        {
            ImGui::SameLine();
            ImGui::Text("(Done)");
        }

        ImGui::Spacing();
    }

    ImGui::End();
}
