#include "ResourcesHUD.hpp"
#include "imgui.h"
#include <format>

void ResourcesHUD::Draw(ResourcesManager& manager)
{
    ImGui::Begin("Resources & Personnel", nullptr, m_flags);

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

void ResourcesHUD::DrawMoney(ResourcesManager& manager)
{
    long money = manager.getMoney();

    ImGui::Text("Money: %ld $", money);

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Available project funds.");
        ImGui::Text("Used for hiring, research, and facility expansion.");
        ImGui::EndTooltip();
    }

    // Add/spend money
    static int adjust = 0;

    ImGui::InputInt("Amount", &adjust);

    if (adjust < 0) adjust = 0;

    if (ImGui::Button("Add Money"))
        manager.addMoney(adjust);

    ImGui::SameLine();

    if (ImGui::Button("Spend Money"))
        manager.spendMoney(adjust);
}

void ResourcesHUD::DrawMaterials(ResourcesManager& manager)
{
    ImGui::Text("Materials");

    // Uranium
    ImGui::Text("Uranium: %u", manager.getUranium());
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Used for enrichment and bomb fuel.");
        ImGui::EndTooltip();
    }

    // Plutonium
    ImGui::Text("Plutonium: %u", manager.getPlutonium());
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Produced in nuclear reactors.");
        ImGui::EndTooltip();
    }

    static int amount = 0;
    ImGui::InputInt("Amount##Material", &amount);

    if (amount < 0) amount = 0;

    if (ImGui::Button("Add Uranium"))
        manager.addUranium(amount);

    ImGui::SameLine();

    if (ImGui::Button("Spend Uranium"))
        manager.spendUranium(amount);

    if (ImGui::Button("Add Plutonium"))
        manager.addPlutonium(amount);

    ImGui::SameLine();

    if (ImGui::Button("Spend Plutonium"))
        manager.spendPlutonium(amount);
}

void ResourcesHUD::DrawPersonnel(ResourcesManager& manager)
{
    ImGui::Text("Personnel");

    auto row = [&](const char* label,
                   unsigned total,
                   unsigned working,
                   unsigned available,
                   unsigned dailyCost,
                   unsigned hireCost,
                   auto hireFunc,
                   auto fireFunc)
    {
        ImGui::Separator();

        ImGui::Text("%s", label);
        ImGui::Text("Total: %u", total);
        ImGui::Text("Working: %u", working);
        ImGui::Text("Available: %u", available);
        ImGui::Text("Daily Cost: %u", dailyCost);

        static int count = 0;

        ImGui::InputInt(std::format("Amount##{}", label).c_str(), &count);
        if (count < 0) count = 0;

        if (ImGui::Button(std::format("Hire##{}", label).c_str()))
            hireFunc(count);

        ImGui::SameLine();

        if (ImGui::Button(std::format("Fire##{}", label).c_str()))
            fireFunc(count);

        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::Text("Cost to hire %s: %u per person", label, hireCost);
            ImGui::EndTooltip();
        }
    };

    row("Workers",
        manager.getTotalWorkers(),
        manager.getWorkingWorkers(),
        manager.getAvailableToHireWorkers(),
        manager.dailyWorkersCost(),
        manager.getResourceConstraints().worker_hiring_cost,
        [&](int c){ manager.hireWorkers(c); },
        [&](int c){ manager.fireWorkers(c); });

    row("Scientists",
        manager.getTotalScientists(),
        manager.getWorkingScientists(),
        manager.getAvailableToHireScientists(),
        manager.dailyScientistsCost(),
        manager.getResourceConstraints().scientist_hiring_cost,
        [&](int c){ manager.hireScientists(c); },
        [&](int c){ manager.fireScientists(c); });

    row("Engineers",
        manager.getTotalEngineers(),
        manager.getWorkingEngineers(),
        manager.getAvailableToHireEngineers(),
        manager.dailyEngineersCost(),
        manager.getResourceConstraints().engineer_hiring_cost,
        [&](int c){ manager.hireEngineers(c); },
        [&](int c){ manager.fireEngineers(c); });

    row("Army Personnel",
        manager.getTotalArmyPersonnel(),
        manager.getWorkingArmyPersonnel(),
        manager.getAvailableToHireArmyPersonnel(),
        manager.dailyArmyPersonnelCost(),
        manager.getResourceConstraints().army_personnel_hiring_cost,
        [&](int c){ manager.hireArmyPersonnel(c); },
        [&](int c){ manager.fireArmyPersonnel(c); });
}

void ResourcesHUD::DrawFacilityStats(ResourcesManager& manager)
{
    ImGui::Text("Facility Stats");

    // ----- Morale -----
    unsigned morale = manager.getMorale();
    float moraleNorm = morale / 100.f;

    ImGui::Text("Morale:");
    ImGui::ProgressBar(moraleNorm, ImVec2(-1, 0), std::format("{}%", morale).c_str());

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Morale reflects overall workforce satisfaction.");
        ImGui::Text("Low morale slows down research and production.");
        ImGui::EndTooltip();
    }

    // adjust morale
    static int moraleAdjust = 0;
    ImGui::InputInt("Adjust Morale", &moraleAdjust);

    if (moraleAdjust < 0) moraleAdjust = 0;

    if (ImGui::Button("Increase Morale"))
        manager.addMorale(moraleAdjust);

    ImGui::SameLine();

    if (ImGui::Button("Decrease Morale"))
        manager.reduceMorale(moraleAdjust);

    ImGui::Spacing();

    // ----- Security -----
    unsigned sec = manager.getSecurity();
    float secNorm = sec / 100.f;

    ImGui::Text("Security:");
    ImGui::ProgressBar(secNorm, ImVec2(-1, 0), std::format("{}%", sec).c_str());

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Security reduces espionage risk.");
        ImGui::Text("Low security increases chance of sabotage.");
        ImGui::EndTooltip();
    }

    static int secAdjust = 0;
    ImGui::InputInt("Adjust Security", &secAdjust);

    if (ImGui::Button("Increase Security"))
        manager.addSecurity(secAdjust);

    ImGui::SameLine();

    if (ImGui::Button("Decrease Security"))
        manager.reduceSecurity(secAdjust);
}
