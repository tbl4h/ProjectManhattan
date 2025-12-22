#include "TechTreeHUD.hpp"

void TechTreeHUD::Draw()
{
    if (!m_visible)
        return;

    if (!ImGui::Begin("Technology Tree", &m_visible, m_flags))
    {
        ImGui::End();
        return;
    }

    const auto& manager = m_controller.Model();

    for (const auto& [id, tech] : manager.getAllTechnologies())
    {
        if (!tech.m_prerequisites.empty())
            continue;

        DrawNode(tech);
    }

    ImGui::End();
}

void TechTreeHUD::DrawNode(const Technology& tech)
{
    const auto& manager = m_controller.Model();

    ImVec4 color =
        tech.isCompleted()   ? ImVec4(0.3f, 1.f, 0.3f, 1.f) :
        tech.isInProgress()  ? ImVec4(1.f, 0.85f, 0.4f, 1.f) :
        manager.isAvailable(tech.m_id)
            ? ImVec4(1.f, 1.f, 1.f, 1.f)
            : ImVec4(0.5f, 0.5f, 0.5f, 1.f);

    ImGui::PushStyleColor(ImGuiCol_Text, color);

    bool open = ImGui::TreeNode(tech.m_name.c_str());

    ImGui::PopStyleColor();

    if (ImGui::IsItemClicked() && manager.isAvailable(tech.m_id))
    {
        m_controller.StartResearch(tech.m_id);
    }

    if (!open)
        return;

    for (const auto& [id, child] : manager.getAllTechnologies())
    {
        for (const auto& pre : child.m_prerequisites)
        {
            if (pre == tech.m_id)
            {
                DrawNode(child);
                break;
            }
        }
    }

    ImGui::TreePop();
}
