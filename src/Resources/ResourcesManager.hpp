#pragma once
#include <string>
#include "../Core/header/TimeSystem.hpp"

using std::shared_ptr;
using std::string;

struct ResourceMissing
{
    bool money = false;
    bool uranium = false;
    bool plutonium = false;

    bool workers = false;
    bool engineers = false;
    bool scientists = false;
    bool army = false;
};

struct ResourceConstraints
{
    // money
    unsigned long real_budget = 440000000;
    unsigned long minimum_budget = 100000000;
    unsigned long maximum_budget = 1000000000;

    unsigned int preliminary_minimum = 20000;
    unsigned int preliminary_maximum = 150000;

    unsigned int infrastructure_minimum = 10000000;
    unsigned int infrastructure_maximum = 20000000;

    unsigned int full_scale_minimum = 60000000;
    unsigned int full_scale_maximum = 80000000;

    unsigned int fussed_fuel_minimum = 100000000;
    unsigned int fussed_fuel_maximum = 120000000;

    unsigned int bomb_assembly_minimum = 50000000;
    unsigned int bomb_assembly_maximum = 60000000;

    unsigned int wind_down_minimum = 4000000;
    unsigned int wind_down_maximum = 6000000;

    // personnel
    unsigned short worker_daily_cost = 1;
    unsigned short worker_hiring_cost = 1;

    unsigned short scientist_daily_cost = 5;
    unsigned short scientist_hiring_cost = 5;
    unsigned short engineer_daily_cost = 2;
    unsigned short engineer_hiring_cost = 2;
    unsigned short army_personnel_daily_cost = 3;
    unsigned short army_personnel_hiring_cost = 3;

    unsigned long total_numbers_of_all_personnel = 130000;

    unsigned int initial_total_workers = 100000;
    unsigned int maximum_total_workers = 1000000;
    unsigned int initial_total_scientists = 3000;
    unsigned int maximum_total_scientists = 50000;
    unsigned int initial_total_engineers = 7000;
    unsigned int maximum_total_engineers = 100000;
    unsigned int initial_total_army_personnel = 20000;
    unsigned int maximum_total_army_personnel = 200000;

    unsigned int initial_money = 10000;
    unsigned int initial_uranium = 0;
    unsigned int initial_plutonium = 0;
    unsigned int initial_morale = 60;
    unsigned int initial_security = 40;
    unsigned int maximal_uranium = 1000000;
    unsigned int maximal_plutonium = 500000;
    unsigned short minimal_total_morale = 0;
    unsigned short maximal_total_morale = 100;
    unsigned short minimal_total_security = 0;
    unsigned short maximal_total_security = 100;

    // method
    bool loadFromJson(const string &path);
};

class ResourcesManager
{
public:
    ResourcesManager(ResourceConstraints &constraints, TimeDataModel &timeSystem);
    ~ResourcesManager() = default;
    // Workers management
    bool hireWorkers(unsigned int count);
    bool fireWorkers(unsigned int count);
    unsigned int getAvailableToHireWorkers() const;
    unsigned int dailyWorkersCost() const;
    unsigned int tenDaysWorkersCost() const;
    unsigned int thirtyDaysWorkersCost() const;
    // Scientists management
    bool hireScientists(unsigned int count);
    bool fireScientists(unsigned int count);
    unsigned int getAvailableToHireScientists() const;
    unsigned int dailyScientistsCost() const;
    unsigned int tenDaysScientistsCost() const;
    unsigned int thirtyDaysScientistsCost() const;
    // Engineers management
    bool hireEngineers(unsigned int count);
    bool fireEngineers(unsigned int count);
    unsigned int getAvailableToHireEngineers() const;
    unsigned int dailyEngineersCost() const;
    unsigned int tenDaysEngineersCost() const;
    unsigned int thirtyDaysEngineersCost() const;
    // Army Personnel management
    bool hireArmyPersonnel(unsigned int count);
    bool fireArmyPersonnel(unsigned int count);
    unsigned int getAvailableToHireArmyPersonnel() const;
    unsigned int dailyArmyPersonnelCost() const;
    unsigned int tenDaysArmyPersonnelCost() const;
    unsigned int thirtyDaysArmyPersonnelCost() const;
    // Check total numbers of all personnel
    bool checkTotalNumbersOfAllPersonnel() const;
    // Reset daily hired personnel counts
    void resetDailyHiredPersonnelCounts();
    // Total hiring costs
    unsigned long dailyPersonnelCost() const;
    unsigned long tenDaysPersonnelCost() const;
    unsigned long thirtyDaysPersonnelCost() const;
    // Getters for personnel counts
    unsigned int getTotalWorkers() const;
    unsigned int getWorkingWorkers() const;
    unsigned int getTotalScientists() const;
    unsigned int getWorkingScientists() const;
    unsigned int getTotalEngineers() const;
    unsigned int getWorkingEngineers() const;
    unsigned int getTotalArmyPersonnel() const;
    unsigned int getWorkingArmyPersonnel() const;
    // Setters for personnel counts
    bool setTotalWorkers(unsigned int count);
    bool setTotalScientists(unsigned int count);
    bool setTotalEngineers(unsigned int count);
    bool setTotalArmyPersonnel(unsigned int count);
    // Resource stats management
    long getMoney() const;
    bool addMoney(long amount);
    bool spendMoney(long amount);
    unsigned int getUranium() const;
    bool addUranium(unsigned int amount);
    bool spendUranium(unsigned int amount);
    unsigned int getPlutonium() const;
    bool addPlutonium(unsigned int amount);
    bool spendPlutonium(unsigned int amount);
    // Facility stats management
    unsigned int getMorale() const;
    bool addMorale(unsigned int amount);
    bool reduceMorale(unsigned int amount);
    unsigned int getSecurity() const;
    bool addSecurity(unsigned int amount);
    bool reduceSecurity(unsigned int amount);
    ResourceConstraints &getResourceConstraints() const;

private:
    void onDayPassed(const TimeDataModel &timeModel);

private:
    ResourceConstraints &m_resourceConstraints;
    TimeDataModel &m_timeModel;
    shared_ptr<TimeDataModel::DayPassedCallback> m_dayObserverHandle;
    // Total amount of workerss possible to hire.
    unsigned int m_totalWorkers;
    // Currently working workers.
    unsigned int m_workingWorkers;
    // Hired workers in the current day. Variable resets at the end of the day.
    unsigned int m_hiredWorkersInDay;
    unsigned int m_totalScientists;
    unsigned int m_workingScientists;
    // Hired scientists in the current day. Variable resets at the end of the day.
    unsigned int m_hiredScientistsInDay;
    unsigned int m_totalEngineers;
    unsigned int m_workingEngineers;
    // Hired engineers in the current day. Variable resets at the end of the day.
    unsigned int m_hiredEngineersInDay;

    unsigned int m_totalArmyPersonnel;
    unsigned int m_workingArmyPersonnel;
    // Hired army personnel in the current day. Variable resets at the end of the day.
    unsigned int m_hiredArmyPersonnelInDay;

    // Resource stats
    // If money is negative, m_morale and m_security decrease faster.
    long m_money;
    unsigned int m_uranium;
    unsigned int m_plutonium;
    // Facility stats
    // If m_totalMorale is 0 game over.
    unsigned int m_totalMorale;
    // If m_totalSecurity is low, risk of espionage increases.
    unsigned int m_totalSecurity;
};