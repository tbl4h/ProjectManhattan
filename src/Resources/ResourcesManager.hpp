#pragma once
#include <string>
#include "../Core/header/TimeSystem.hpp"
#include "ResourceMissing.hpp"
#include "ResourceConstraints.hpp"

using std::shared_ptr;
using std::string;

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