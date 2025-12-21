#pragma once
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <functional>
#include <unordered_map>
#include "./../Core/header/TimeSystem.hpp"
#include "./../Resources/ResourcesManager.hpp"


using std::enable_shared_from_this;
using std::function;
using std::optional;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;

enum class TechnologyType
{
    Theory,
    Engineering,
};

enum class ResearchState
{
    Locked,
    Available,
    InProgress,
    Completed
};



struct Technology
{
    string m_id;
    string m_name;
    TechnologyType m_type;
    unsigned short m_researchDays = 0;
    vector<string> m_prerequisites;
    string m_description;
    unsigned m_moneyCost = 0;
    unsigned m_daylyCost = 0;
    unsigned m_uraniumRequired = 0;
    unsigned m_plutoniumRequired = 0;
    unsigned m_workersRequired = 0;
    unsigned m_engineersRequired = 0;
    unsigned m_scientistsRequired = 0;
    unsigned m_armyPersonnelRequired = 0;
    vector<string> m_buildingRequired;
    vector<string> m_charactersInvolved;

    // dynamic state
    ResearchState m_state = ResearchState::Locked;
    unsigned m_progressDays = 0;

    inline bool isCompleted() const { return m_state == ResearchState::Completed; }
    inline bool isAvailable() const { return m_state == ResearchState::Available; }
    inline bool isInProgress() const { return m_state == ResearchState::InProgress; }
};

class ResearchManager : public enable_shared_from_this<ResearchManager>
{
public:
    using ResearchCompletedCallback =
        function<void(const Technology &)>;

    using ResearchMissingResourcesCallback =
        function<void(const ResourceMissing &, const Technology &)>;

    ResearchManager(TimeDataModel &timeModel, ResourcesManager &resources);
    ~ResearchManager() = default;

    void loadFromJson(const string &path);

    bool startResearch(const string &techId);
    void onDayPassed(const TimeDataModel &time);

    bool isCompleted(const string &techId) const;
    bool isAvailable(const string &techId) const;
    float getProgress(const string &techId) const;

    const unordered_map<string, Technology> &
    getAllTechnologies() const { return m_techs; }
    const Technology *getActiveResearch() const;

    void addResearchCompletedListener(ResearchCompletedCallback cb);
    void addResearchMissingResourcesListener(ResearchMissingResourcesCallback cb);

private:
    void updateAvailability();
    void calculateResearchTime(Technology &tech);

private:
    TimeDataModel &m_timeModel;
    ResourcesManager &m_resources;
    unordered_map<string, Technology> m_techs;
    shared_ptr<TimeDataModel::DayPassedCallback> m_dayObserverHandle;

    optional<string> m_activeResearchId;

    vector<ResearchCompletedCallback> m_researchCompletedListeners;
    vector<ResearchMissingResourcesCallback> m_missingResourcesListeners;
};
