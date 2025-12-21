#include "ResearchManager.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using std::make_shared;
using json = nlohmann::json;
using std::ifstream;
using std::move;

ResearchManager::ResearchManager(TimeDataModel& timeModel, ResourcesManager& resources)
    : m_timeModel(timeModel), m_resources(resources)
{
    // Register as observer
    m_dayObserverHandle = make_shared<TimeDataModel::DayPassedCallback>(
        [this](const TimeDataModel& t) { onDayPassed(t); }
    );

    m_timeModel.addDayObserver(m_dayObserverHandle);
}

void ResearchManager::loadFromJson(const string& path)
{
    ifstream file(path);
    json data = json::parse(file);

    for (auto& t : data["technologies"])
    {
        Technology tech;
        tech.m_id = t["id"];
        tech.m_name = t["name"];
        tech.m_type =
            (t["type"] == "engineering" ? TechnologyType::Engineering : TechnologyType::Theory);
        tech.m_researchDays = t["research_days"];
        tech.m_description = t["description"];
        tech.m_moneyCost = t["money_cost"];
        tech.m_daylyCost = t["dayly_cost"];

        for (auto& pre : t["prerequisites"])
            tech.m_prerequisites.push_back(pre);

        tech.m_uraniumRequired = t.value("uranium_required", 0u);
        tech.m_plutoniumRequired = t.value("plutonium_required", 0u);
        tech.m_workersRequired = t.value("workers_required", 0u);
        tech.m_engineersRequired = t.value("engineers_required", 0u);
        tech.m_scientistsRequired = t.value("scientists_required", 0u);
        tech.m_armyPersonnelRequired = t.value("army_personnel_required", 0u);

        for (auto& building : t["building_required"])
            tech.m_buildingRequired.push_back(building);

        for (auto& character : t["characters_involved"])
            tech.m_charactersInvolved.push_back(character);

        m_techs[tech.m_id] = tech;
        
    }

    updateAvailability();
}

void ResearchManager::updateAvailability()
{
    for (auto& [id, tech] : m_techs)
    {
        if (tech.isCompleted()) continue;
        if (tech.isInProgress()) continue;

        bool allDone = true;
        for (auto& pre : tech.m_prerequisites)
        {
            auto it = m_techs.find(pre);
            if (it == m_techs.end() || !it->second.isCompleted())
            {
                allDone = false;
                break;
            }
        }

        if (allDone)
            tech.m_state = ResearchState::Available;
    }
}

bool ResearchManager::startResearch(const string& techId)
{
    auto it = m_techs.find(techId);
    if (it == m_techs.end())
        return false;

    Technology& tech = it->second;

    if (!tech.isAvailable() && !tech.isInProgress())
        return false;

    ResourceMissing missing;

    if (m_resources.getMoney() < tech.m_moneyCost)
        missing.money = true;

    if (m_resources.getUranium() < tech.m_uraniumRequired)
        missing.uranium = true;

    if (m_resources.getPlutonium() < tech.m_plutoniumRequired)
        missing.plutonium = true;
    
    if (m_resources.getWorkingScientists() < tech.m_scientistsRequired)
        missing.scientists = true;

    if (m_resources.getWorkingEngineers() < tech.m_engineersRequired)
        missing.engineers = true;

    if (m_resources.getWorkingWorkers() < tech.m_workersRequired)
        missing.workers = true;

    if (m_resources.getWorkingArmyPersonnel() < tech.m_armyPersonnelRequired)
        missing.army = true;

    // Jeśli cokolwiek brakuje → event + abort
    if (missing.money || missing.uranium || missing.plutonium ||
        missing.scientists || missing.engineers ||
        missing.workers || missing.army)
    {
        for (auto& cb : m_missingResourcesListeners)
            cb(missing, tech);

        return false;
    }

    // koszt jednorazowy
    if (!tech.isInProgress())
    {
        m_resources.spendMoney(tech.m_moneyCost);
        tech.m_state = ResearchState::InProgress;
    }

    m_activeResearchId = tech.m_id;
    return true;
}


void ResearchManager::onDayPassed(const TimeDataModel&)
{
    if (!m_activeResearchId.has_value())
        return;

    auto it = m_techs.find(*m_activeResearchId);
    if (it == m_techs.end())
    {
        m_activeResearchId.reset();
        return;
    }

    Technology& tech = it->second;
    tech.m_progressDays++;

    if (tech.m_progressDays >= tech.m_researchDays)
    {
        tech.m_state = ResearchState::Completed;
        // Notify listeners
        for (auto& cb : m_researchCompletedListeners)
            cb(tech);
        m_activeResearchId.reset();
        updateAvailability();
    }
}


bool ResearchManager::isCompleted(const string& techId) const
{
    auto it = m_techs.find(techId);
    return it != m_techs.end() && it->second.isCompleted();
}

bool ResearchManager::isAvailable(const string& techId) const
{
    auto it = m_techs.find(techId);
    return it != m_techs.end() && it->second.isAvailable();
}

float ResearchManager::getProgress(const string& techId) const
{
    auto it = m_techs.find(techId);
    if (it == m_techs.end()) return 0.f;

    if (!it->second.isInProgress()) return 0.f;

    return float(it->second.m_progressDays) / float(it->second.m_researchDays);
}

const Technology* ResearchManager::getActiveResearch() const
{
    if (!m_activeResearchId.has_value())
        return nullptr;

    auto it = m_techs.find(*m_activeResearchId);
    if (it == m_techs.end())
        return nullptr;

    return &it->second;
}

void ResearchManager::calculateResearchTime(Technology& tech)
{
    // Placeholder for any complex calculations in the future
    // Currently, research time is static as defined in the JSON
}

void ResearchManager::addResearchCompletedListener(
    ResearchCompletedCallback cb)
{
    m_researchCompletedListeners.push_back(move(cb));
}

void ResearchManager::addResearchMissingResourcesListener(
    ResearchMissingResourcesCallback cb)
{
    m_missingResourcesListeners.push_back(std::move(cb));
}