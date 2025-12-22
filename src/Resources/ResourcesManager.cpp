#include "ResourcesManager.hpp"
#include <algorithm>

using std::clamp;
using std::min;
using std::make_shared;


ResourcesManager::ResourcesManager(ResourceConstraints &constraints, TimeDataModel &timeSystem)
    : m_resourceConstraints(constraints),
      m_totalWorkers(constraints.initial_total_workers),
      m_workingWorkers(0),
      m_hiredWorkersInDay(0),
      m_totalScientists(constraints.initial_total_scientists),
      m_workingScientists(0),
      m_hiredScientistsInDay(0),
      m_totalEngineers(constraints.initial_total_engineers),
      m_workingEngineers(0),
      m_hiredEngineersInDay(0),
      m_totalArmyPersonnel(constraints.initial_total_army_personnel),
      m_workingArmyPersonnel(0),
      m_hiredArmyPersonnelInDay(0),
      m_money(constraints.initial_money),
      m_uranium(constraints.initial_uranium),
      m_plutonium(constraints.initial_plutonium),
      m_totalMorale(constraints.initial_morale),
      m_totalSecurity(constraints.initial_security),
      m_timeModel(timeSystem)    
{
    m_dayObserverHandle =
        make_shared<TimeDataModel::DayPassedCallback>(
            [this](const TimeDataModel& t)
            {
                onDayPassed(t);
            });

    m_timeModel.addDayObserver(m_dayObserverHandle);
}

void ResourcesManager::onDayPassed(const TimeDataModel&)
{
    // 1️⃣ Dzienne koszty personelu
    long dailyCost =
        dailyWorkersCost() +
        dailyScientistsCost() +
        dailyEngineersCost() +
        dailyArmyPersonnelCost();

    m_money -= dailyCost;

    // 2️⃣ Jeśli nie stać nas na utrzymanie
    if (m_money <= 0)
    {
        // morale spada szybciej
        reduceMorale(2);
        reduceSecurity(1);
    }
    else
    {
        // stabilny projekt
        addMorale(1);
    }

    // 3️⃣ Reset liczników dziennych
    resetDailyHiredPersonnelCounts();
}


bool ResourcesManager::hireWorkers(unsigned int count)
{
    unsigned int available = getAvailableToHireWorkers();
    unsigned long cost = count * m_resourceConstraints.worker_hiring_cost;

    if (available >= count && m_money >= cost)
    {
        m_workingWorkers += count;
        m_hiredWorkersInDay += count;
        m_money -= cost;
        return true;
    }
    return false;
}

bool ResourcesManager::fireWorkers(unsigned int count)
{
    if (m_workingWorkers >= count)
    {
        m_workingWorkers -= count;
        return true;
    }
    return false;
}

unsigned int ResourcesManager::getAvailableToHireWorkers() const
{
    return m_totalWorkers - m_workingWorkers;
}

unsigned int ResourcesManager::dailyWorkersCost() const
{
    unsigned int totalCost = 0;
    totalCost += m_workingWorkers * m_resourceConstraints.worker_daily_cost;
    return totalCost;
}

unsigned int ResourcesManager::tenDaysWorkersCost() const
{
    unsigned int totalCost = dailyWorkersCost() * 10;
    return totalCost;
}

unsigned int ResourcesManager::thirtyDaysWorkersCost() const
{
    unsigned int totalCost = dailyWorkersCost() * 30;
    return totalCost;
}

bool ResourcesManager::hireScientists(unsigned int count)
{
    unsigned int available = getAvailableToHireScientists();
    unsigned long cost = count * m_resourceConstraints.scientist_hiring_cost;
    if (available >= count && m_money >= cost)
    {
        m_workingScientists += count;
        m_hiredScientistsInDay += count;
        m_money -= cost;
        return true;
    }
    return false;
}

bool ResourcesManager::fireScientists(unsigned int count)
{
    if (m_workingScientists >= count)
    {
        m_workingScientists -= count;
        return true;
    }
    return false;
}

unsigned int ResourcesManager::getAvailableToHireScientists() const
{
    return m_totalScientists - m_workingScientists;
}

unsigned int ResourcesManager::dailyScientistsCost() const
{
    unsigned int totalCost = 0;
    totalCost += m_workingScientists * m_resourceConstraints.scientist_daily_cost;
    return totalCost;
}

unsigned int ResourcesManager::tenDaysScientistsCost() const
{
    unsigned int totalCost = dailyScientistsCost() * 10;
    return totalCost;
}

unsigned int ResourcesManager::thirtyDaysScientistsCost() const
{
    unsigned int totalCost = dailyScientistsCost() * 30;
    return totalCost;
}

bool ResourcesManager::hireEngineers(unsigned int count)
{
    unsigned int available = getAvailableToHireEngineers();
    unsigned long cost = count * m_resourceConstraints.engineer_hiring_cost;
    if (available >= count && m_money >= cost)
    {
        m_workingEngineers += count;
        m_hiredEngineersInDay += count;
        m_money -= cost;
        return true;
    }
    return false;
}

bool ResourcesManager::fireEngineers(unsigned int count)
{
    if (m_workingEngineers >= count)
    {
        m_workingEngineers -= count;
        return true;
    }
    return false;
}

unsigned int ResourcesManager::getAvailableToHireEngineers() const
{
    return m_totalEngineers - m_workingEngineers;
}

unsigned int ResourcesManager::dailyEngineersCost() const
{
    unsigned int totalCost = 0;
    totalCost += m_workingEngineers * m_resourceConstraints.engineer_daily_cost;
    return totalCost;
}

unsigned int ResourcesManager::tenDaysEngineersCost() const
{
    unsigned int totalCost = dailyEngineersCost() * 10;
    return totalCost;
}

unsigned int ResourcesManager::thirtyDaysEngineersCost() const
{
    unsigned int totalCost = dailyEngineersCost() * 30;
    return totalCost;
}

bool ResourcesManager::hireArmyPersonnel(unsigned int count)
{
    unsigned int available = getAvailableToHireArmyPersonnel();
    unsigned long cost = count * m_resourceConstraints.army_personnel_hiring_cost;
    if (available >= count && m_money >= cost)
    {
        m_workingArmyPersonnel += count;
        m_hiredArmyPersonnelInDay += count;
        m_money -= cost;
        return true;
    }
    return false;
}

bool ResourcesManager::fireArmyPersonnel(unsigned int count)
{
    if (m_workingArmyPersonnel >= count)
    {
        m_workingArmyPersonnel -= count;
        return true;
    }
    return false;
}

unsigned int ResourcesManager::getAvailableToHireArmyPersonnel() const
{
    return m_totalArmyPersonnel - m_workingArmyPersonnel;
}

unsigned int ResourcesManager::dailyArmyPersonnelCost() const
{
    unsigned int totalCost = 0;
    totalCost += m_workingArmyPersonnel * m_resourceConstraints.army_personnel_daily_cost;
    return totalCost;
}
unsigned int ResourcesManager::tenDaysArmyPersonnelCost() const
{
    unsigned int totalCost = dailyArmyPersonnelCost() * 10;
    return totalCost;
}
unsigned int ResourcesManager::thirtyDaysArmyPersonnelCost() const
{
    unsigned int totalCost = dailyArmyPersonnelCost() * 30;
    return totalCost;
}

bool ResourcesManager::checkTotalNumbersOfAllPersonnel() const
{
    unsigned int currentTotal = m_totalWorkers + m_totalScientists + m_totalEngineers + m_totalArmyPersonnel;
    if (currentTotal <= m_resourceConstraints.total_numbers_of_all_personnel)
        return true;
    return false;
}

void ResourcesManager::resetDailyHiredPersonnelCounts()
{
    m_hiredWorkersInDay = 0;
    m_hiredScientistsInDay = 0;
    m_hiredEngineersInDay = 0;
    m_hiredArmyPersonnelInDay = 0;
}

unsigned long ResourcesManager::dailyPersonnelCost() const
{
    unsigned long totalCost = 0;
    totalCost += dailyWorkersCost();
    totalCost += dailyScientistsCost();
    totalCost += dailyEngineersCost();
    totalCost += dailyArmyPersonnelCost();
    return totalCost;
}
unsigned long ResourcesManager::tenDaysPersonnelCost() const
{
    unsigned long totalCost = 0;
    totalCost += tenDaysWorkersCost();
    totalCost += tenDaysScientistsCost();
    totalCost += tenDaysEngineersCost();
    totalCost += tenDaysArmyPersonnelCost();
    return totalCost;
}
unsigned long ResourcesManager::thirtyDaysPersonnelCost() const
{
    unsigned long totalCost = 0;
    totalCost += thirtyDaysWorkersCost();
    totalCost += thirtyDaysScientistsCost();
    totalCost += thirtyDaysEngineersCost();
    totalCost += thirtyDaysArmyPersonnelCost();
    return totalCost;
}
// Getters for personnel counts
unsigned int ResourcesManager::getTotalWorkers() const
{
    return m_totalWorkers;
}
unsigned int ResourcesManager::getWorkingWorkers() const
{
    return m_workingWorkers;
}
unsigned int ResourcesManager::getTotalScientists() const
{
    return m_totalScientists;
}
unsigned int ResourcesManager::getWorkingScientists() const
{
    return m_workingScientists;
}
unsigned int ResourcesManager::getTotalEngineers() const
{
    return m_totalEngineers;
}
unsigned int ResourcesManager::getWorkingEngineers() const
{
    return m_workingEngineers;
}
unsigned int ResourcesManager::getTotalArmyPersonnel() const
{
    return m_totalArmyPersonnel;
}
unsigned int ResourcesManager::getWorkingArmyPersonnel() const
{
    return m_workingArmyPersonnel;
}
// Setters for personnel counts
bool ResourcesManager::setTotalWorkers(unsigned int count)
{
    unsigned int otherTotals = m_totalScientists + m_totalEngineers + m_totalArmyPersonnel;
    if (count + otherTotals > m_resourceConstraints.total_numbers_of_all_personnel)
        return false;
    if (count > m_resourceConstraints.maximum_total_workers)
        return false;
    m_totalWorkers = count;
    return true;
}

bool ResourcesManager::setTotalScientists(unsigned int count)
{
    unsigned int otherTotals = m_totalWorkers + m_totalEngineers + m_totalArmyPersonnel;
    if (count + otherTotals > m_resourceConstraints.total_numbers_of_all_personnel)
        return false;
    if (count > m_resourceConstraints.maximum_total_scientists)
        return false;
    m_totalScientists = count;
    return true;
}

bool ResourcesManager::setTotalEngineers(unsigned int count)
{
    unsigned int otherTotals = m_totalWorkers + m_totalScientists + m_totalArmyPersonnel;
    if (count + otherTotals > m_resourceConstraints.total_numbers_of_all_personnel)
        return false;
    if (count > m_resourceConstraints.maximum_total_engineers)
        return false;
    m_totalEngineers = count;
    return true;
}
bool ResourcesManager::setTotalArmyPersonnel(unsigned int count)
{
    unsigned int otherTotals = m_totalWorkers + m_totalScientists + m_totalEngineers;
    if (count + otherTotals > m_resourceConstraints.total_numbers_of_all_personnel)
        return false;
    if (count > m_resourceConstraints.maximum_total_army_personnel)
        return false;
    m_totalArmyPersonnel = count;
    return true;
}
// Resource stats management
long ResourcesManager::getMoney() const
{
    return m_money;
}
bool ResourcesManager::addMoney(long amount)
{
    if (amount < 0)
        return false;
    m_money = min(m_money + amount, (long)m_resourceConstraints.maximum_budget);
    return true;
}
bool ResourcesManager::spendMoney(long amount)
{
    if (m_money >= amount)
    {
        m_money -= amount;
        return true;
    }
    return false;
}

unsigned int ResourcesManager::getUranium() const
{
    return m_uranium;
}
bool ResourcesManager::addUranium(unsigned int amount)
{
    m_uranium = std::min(m_uranium + amount, m_resourceConstraints.maximal_uranium);
    return true;
}
bool ResourcesManager::spendUranium(unsigned int amount)
{
    if (m_uranium >= amount)
    {
        m_uranium -= amount;
        return true;
    }
    return false;
}
unsigned int ResourcesManager::getPlutonium() const
{
    return m_plutonium;
}
bool ResourcesManager::addPlutonium(unsigned int amount)
{
    m_plutonium = std::min(m_plutonium + amount, m_resourceConstraints.maximal_plutonium);
    return true;
}
bool ResourcesManager::spendPlutonium(unsigned int amount)
{
    if (m_plutonium >= amount)
    {
        m_plutonium -= amount;
        return true;
    }
    return false;
}
// Facility stats management
unsigned int ResourcesManager::getMorale() const
{
    return m_totalMorale;
}
bool ResourcesManager::addMorale(unsigned int amount)
{
    unsigned int oldValue = m_totalMorale;

    m_totalMorale = clamp(
        m_totalMorale + amount,
        (unsigned int)m_resourceConstraints.minimal_total_morale,
        (unsigned int)m_resourceConstraints.maximal_total_morale);

    return m_totalMorale != oldValue; // true jeśli faktycznie zmieniło
}

bool ResourcesManager::reduceMorale(unsigned int amount)
{
    unsigned int oldValue = m_totalMorale;

    // Jeśli amount > obecna wartość → clamp ustawi minimal
    m_totalMorale = clamp(
        (amount > m_totalMorale) ? 0u : m_totalMorale - amount,
        (unsigned int)m_resourceConstraints.minimal_total_morale,
        (unsigned int)m_resourceConstraints.maximal_total_morale);

    return m_totalMorale != oldValue;
}

unsigned int ResourcesManager::getSecurity() const
{
    return m_totalSecurity;
}
bool ResourcesManager::addSecurity(unsigned int amount)
{
    unsigned int oldValue = m_totalSecurity;

    m_totalSecurity = clamp(
        m_totalSecurity + amount,
        (unsigned int)m_resourceConstraints.minimal_total_security,
        (unsigned int)m_resourceConstraints.maximal_total_security);

    return m_totalSecurity != oldValue;
}

bool ResourcesManager::reduceSecurity(unsigned int amount)
{
    unsigned int oldValue = m_totalSecurity;

    m_totalSecurity = clamp(
        (amount > m_totalSecurity) ? 0u : m_totalSecurity - amount,
        (unsigned int)m_resourceConstraints.minimal_total_security,
        (unsigned int)m_resourceConstraints.maximal_total_security);

    return m_totalSecurity != oldValue;
}

ResourceConstraints& ResourcesManager::getResourceConstraints() const
{
    return m_resourceConstraints;
}

/*
TODO:
poprawić i ujednolicić pisownie metod maximum
zastanowić się jak będzie wyglądał przepływ danych z tego systemu w grze.
*/