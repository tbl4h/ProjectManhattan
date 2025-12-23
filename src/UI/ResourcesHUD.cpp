#include "ResourcesHUD.hpp"
#include "imgui.h"
#include <format>

ResourcesHUD::ResourcesHUD()
{
    m_highlightUntil = std::chrono::steady_clock::time_point::min();
}

// =====================================================
// HIGHLIGHT HELPERS
// =====================================================
void ResourcesHUD::HighlightIf(bool condition)
{
    if (condition)
    {
        ImGui::PushStyleColor(
            ImGuiCol_Text,
            ImVec4(1.0f, 0.3f, 0.3f, 1.0f)); // czerwony tekst
    }
}

void ResourcesHUD::EndHighlightIf(bool condition)
{
    if (condition)
    {
        ImGui::PopStyleColor();
    }
}

// =====================================================
// MAIN DRAW
// =====================================================
void ResourcesHUD::Draw(ResourcesManager &manager)
{
    UpdateHighlightTimer();

    if (!m_visible)
        return;

    ImGui::Begin("Resources & Personnel", &m_visible, m_flags);

    if (!m_visible)
    {
        ImGui::End();
        return;
    }

    ImGui::Text("Project Resource Management");
    ImGui::Separator();
    ImGui::Spacing();

    DrawMoney(manager);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    DrawMaterials(manager);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    DrawPersonnel(manager);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    DrawFacilityStats(manager);

    ImGui::End();
}

// =====================================================
// MONEY
// =====================================================
void ResourcesHUD::DrawMoney(ResourcesManager &manager)
{
    static int moneyAmount = 1000;

    HighlightIf(m_misingResources.money);
    ImGui::Text("Money: %ld $", manager.getMoney());
    EndHighlightIf(m_misingResources.money);

    ImGui::InputInt("Amount##money", &moneyAmount);
    if (moneyAmount < 0)
        moneyAmount = 0;

    if (ImGui::Button("Add money"))
        manager.addMoney(moneyAmount);

    ImGui::SameLine();

    if (ImGui::Button("Spend money"))
        manager.spendMoney(moneyAmount);
}

// =====================================================
// MATERIALS
// =====================================================
void ResourcesHUD::DrawMaterials(ResourcesManager &manager)
{
    ImGui::Text("Materials");

    HighlightIf(m_misingResources.uranium);
    ImGui::Text("Uranium: %u", manager.getUranium());
    EndHighlightIf(m_misingResources.uranium);

    HighlightIf(m_misingResources.plutonium);
    ImGui::Text("Plutonium: %u", manager.getPlutonium());
    EndHighlightIf(m_misingResources.plutonium);
}

// =====================================================
// PERSONNEL
// =====================================================
void ResourcesHUD::DrawPersonnel(ResourcesManager &manager)
{
    ImGui::Text("Personnel");

    auto row = [&](const char *label,
                   bool highlight,
                   unsigned total,
                   unsigned working,
                   unsigned available,
                   int &inputValue,
                   auto hireFn,
                   auto fireFn)
    {
        HighlightIf(highlight);

        ImGui::Separator();
        ImGui::Text("%s", label);
        ImGui::Text("Total: %u", total);
        ImGui::Text("Working: %u", working);
        ImGui::Text("Available: %u", available);

        ImGui::PushID(label);

        ImGui::InputInt("Amount", &inputValue);
        inputValue = std::max(0, inputValue);

        if (ImGui::Button("Hire"))
        {
            if (inputValue > 0)
                hireFn(static_cast<unsigned>(inputValue));
        }

        ImGui::SameLine();

        if (ImGui::Button("Fire"))
        {
            if (inputValue > 0)
                fireFn(static_cast<unsigned>(inputValue));
        }

        ImGui::PopID();

        EndHighlightIf(highlight);
    };

    // --- Workers ---
    static int workersInput = 0;
    row("Workers", m_misingResources.workers, manager.getTotalWorkers(), manager.getWorkingWorkers(), manager.getAvailableToHireWorkers(), workersInput, [&](unsigned v)
        { manager.hireWorkers(v); }, [&](unsigned v)
        { manager.fireWorkers(v); });

    // --- Scientists ---
    static int scientistsInput = 0;
    row("Scientists", m_misingResources.scientists, manager.getTotalScientists(), manager.getWorkingScientists(), manager.getAvailableToHireScientists(), scientistsInput, [&](unsigned v)
        { manager.hireScientists(v); }, [&](unsigned v)
        { manager.fireScientists(v); });

    // --- Engineers ---
    static int engineersInput = 0;
    row("Engineers", m_misingResources.engineers, manager.getTotalEngineers(), manager.getWorkingEngineers(), manager.getAvailableToHireEngineers(), engineersInput, [&](unsigned v)
        { manager.hireEngineers(v); }, [&](unsigned v)
        { manager.fireEngineers(v); });

    // --- Army ---
    static int armyInput = 0;
    row("Army Personnel", m_misingResources.army, manager.getTotalArmyPersonnel(), manager.getWorkingArmyPersonnel(), manager.getAvailableToHireArmyPersonnel(), armyInput, [&](unsigned v)
        { manager.hireArmyPersonnel(v); }, [&](unsigned v)
        { manager.fireArmyPersonnel(v); });
}

// =====================================================
// FACILITY STATS
// =====================================================
void ResourcesHUD::DrawFacilityStats(ResourcesManager &manager)
{
    ImGui::Text("Facility Stats");

    float morale = manager.getMorale() / 100.0f;
    ImGui::Text("Morale");
    ImGui::ProgressBar(morale, ImVec2(-1, 0));

    float security = manager.getSecurity() / 100.0f;
    ImGui::Text("Security");
    ImGui::ProgressBar(security, ImVec2(-1, 0));
}

// =====================================================
// EVENT FROM RESEARCH MANAGER
// =====================================================
void ResourcesHUD::OnResearchMissingResources(
    const ResourceMissing &missing)
{
    m_misingResources = missing;

    m_highlightUntil =
        std::chrono::steady_clock::now() +
        std::chrono::seconds(1);
}

// =====================================================
// TIMER
// =====================================================
void ResourcesHUD::UpdateHighlightTimer()
{
    if (std::chrono::steady_clock::now() > m_highlightUntil)
    {
        m_misingResources = ResourceMissing{};
    }
}
