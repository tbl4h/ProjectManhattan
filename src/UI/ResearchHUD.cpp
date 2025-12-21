#include "ResearchHUD.hpp"
#include "imgui.h"
#include <format>

#include "ResearchHUD.hpp"
#include <format>

void ResearchHUD::Draw(ResearchManager &manager)
{
    if (!m_visible)
        return;

    bool open = true;

    ImGui::Begin("Research", nullptr, m_flags);

    if (!open)
    {
        m_visible = false;
        ImGui::End();
        return;
    }

    ImGui::Text("Research Tree");
    ImGui::Separator();
    ImGui::Spacing();

    // ============================================================
    // 1. ACTIVE RESEARCH (TOP PANEL)
    // ============================================================
    if (const Technology *active = manager.getActiveResearch())
    {
        float progress =
            static_cast<float>(active->m_progressDays) /
            static_cast<float>(active->m_researchDays);

        ImGui::Text("Currently researching:");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.f, 0.85f, 0.4f, 1.f),
                           "%s", active->m_name.c_str());

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram,
                              ImVec4(0.2f, 0.8f, 0.3f, 1.0f));

        ImGui::ProgressBar(
            progress,
            ImVec2(-1.f, 0.f),
            std::format("{:.0f}%%", progress * 100.f).c_str());

        ImGui::PopStyleColor();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }
    else
    {
        ImGui::TextDisabled("No active research");
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }

    // ============================================================
    // 2. TECHNOLOGY LIST
    // ============================================================
    for (const auto &[id, tech] : manager.getAllTechnologies())
    {
        bool isCompleted = tech.isCompleted();
        bool isInProgress = tech.isInProgress();
        bool isAvailable = manager.isAvailable(id);

        bool isLocked = !isAvailable && !isCompleted && !isInProgress;

        // --------------------------------------------------------
        // COLOR SELECTION
        // --------------------------------------------------------
        if (isCompleted)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 1.f, 0.3f, 1.f)); // green
        else if (isInProgress)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 0.85f, 0.4f, 1.f)); // yellow
        else if (isLocked)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.f)); // gray
        else
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f)); // white

        // --------------------------------------------------------
        // BUTTON
        // --------------------------------------------------------
        bool disableButton = isLocked || isCompleted;

        if (disableButton)
            ImGui::BeginDisabled();

        bool clicked = ImGui::Button(
            tech.m_name.c_str(),
            ImVec2(-1.f, 0.f));

        if (disableButton)
            ImGui::EndDisabled();

        ImGui::PopStyleColor();

        // --------------------------------------------------------
        // TOOLTIP (AFTER BUTTON!)
        // --------------------------------------------------------
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.f);

            ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.4f, 1.f),
                               "%s", tech.m_name.c_str());
            ImGui::Separator();

            ImGui::TextWrapped("%s", tech.m_description.c_str());

            ImGui::Spacing();
            ImGui::Text("Cost: %u $", tech.m_moneyCost);
            ImGui::Text("Duration: %u days", tech.m_researchDays);

            if (!tech.m_prerequisites.empty())
            {
                ImGui::Separator();
                ImGui::Text("Requires:");
                for (const auto &pre : tech.m_prerequisites)
                    ImGui::BulletText("%s", pre.c_str());
            }

            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }

        // --------------------------------------------------------
        // CLICK HANDLING
        // --------------------------------------------------------
        if (clicked && !disableButton)
        {
            manager.startResearch(id);
        }

        // --------------------------------------------------------
        // COMPLETED TAG
        // --------------------------------------------------------
        if (isCompleted)
        {
            ImGui::SameLine();
            ImGui::TextDisabled("(Done)");
        }

        // --------------------------------------------------------
        // PER-TECH PROGRESS BAR
        // --------------------------------------------------------
        if (isInProgress)
        {
            float p =
                static_cast<float>(tech.m_progressDays) /
                static_cast<float>(tech.m_researchDays);

            ImGui::PushStyleColor(ImGuiCol_PlotHistogram,
                                  ImVec4(1.f, 0.6f, 0.2f, 1.f));

            ImGui::ProgressBar(p, ImVec2(-1.f, 0.f));

            ImGui::PopStyleColor();
        }

        ImGui::Spacing();
    }

    ImGui::End();
}

void ResearchHUD::DrawTechTree(ResearchManager &manager)
{
    ImGui::Begin("Technology Tree", nullptr, m_flags);

    ImGui::Text("Tech Tree (auto-generated)");
    ImGui::Separator();
    ImGui::Spacing();

    const auto &techs = manager.getAllTechnologies();

    for (const auto &[id, tech] : techs)
    {
        // Root = technologie bez prerequisite
        bool isRoot = tech.m_prerequisites.empty();

        if (!isRoot)
            continue;

        // rysujemy węzeł główny
        DrawTechNode(manager, tech);
    }

    ImGui::End();
    DrawResearchCompletedPopup();
}

void ResearchHUD::DrawResearchCompletedPopup()
{
    if (m_showPopup)
    {
        ImGui::OpenPopup("Research Completed");
        m_showPopup = false; // reset po otwarciu
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

void ResearchHUD::DrawTechNode(ResearchManager &manager, const Technology &tech)
{
    // kolorowanie węzłów
    ImVec4 color;
    if (tech.isCompleted())
        color = ImVec4(0.3f, 1.f, 0.3f, 1.f); // zielony
    else if (tech.isInProgress())
        color = ImVec4(1.f, 0.85f, 0.4f, 1.f); // żółty
    else if (!tech.isAvailable())
        color = ImVec4(0.5f, 0.5f, 0.5f, 1.f); // szary
    else
        color = ImVec4(1.f, 1.f, 1.f, 1.f); // biały

    ImGui::PushStyleColor(ImGuiCol_Text, color);

    // otwórz / zamknij gałąź
    bool open = ImGui::TreeNodeEx(
        tech.m_name.c_str(),
        ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_SpanAvailWidth |
            (tech.isInProgress() ? ImGuiTreeNodeFlags_DefaultOpen : 0));

    ImGui::PopStyleColor();

    // === TOOLTIP ===
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35);

        ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.4f, 1.f), "%s", tech.m_name.c_str());
        ImGui::Separator();
        ImGui::TextWrapped("%s", tech.m_description.c_str());

        // pokaż prerequisite
        if (!tech.m_prerequisites.empty())
        {
            ImGui::Separator();
            ImGui::Text("Requires:");
            for (auto &pre : tech.m_prerequisites)
            {
                ImGui::BulletText("%s", pre.c_str());
            }
        }

        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }

    // Po kliknięciu na węzeł → rozpocznij badanie
    if (ImGui::IsItemClicked() && manager.isAvailable(tech.m_id))
    {
        manager.startResearch(tech.m_id);
    }

    if (!open)
        return;

    // Rekurencyjnie szukamy technologii które mają ten node jako prerequisite
    for (auto &[id, t] : manager.getAllTechnologies())
    {
        for (auto &pre : t.m_prerequisites)
        {
            if (pre == tech.m_id)
            {
                DrawTechNode(manager, t); // <--- rekurencja
                break;
            }
        }
    }

    ImGui::TreePop();
}

void ResearchHUD::RegisterListeners(ResearchManager &manager)
{
    manager.addResearchCompletedListener(
        [this](const Technology &tech)
        {
            m_completedTechName = tech.m_name;
            m_showPopup = true;
        });
}
