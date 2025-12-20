#include "../header/TimeSystem.hpp"
#include <stdexcept>

using std::move;
using std::range_error;

DateModel::DateModel() : m_day(MIN_DATA_DAY), m_month(MIN_DATA_MONTH), m_year(MIN_DATA_YEAR) {};

DateModel::DateModel(unsigned short a_day, unsigned short a_month, unsigned short a_year)
{
    if (a_year < MIN_DATA_YEAR || a_year > MAX_DATA_YEAR)
        throw range_error("Year out of range");

    if (a_month < MIN_DATA_MONTH || a_month > MAX_DATA_MONTH)
        throw range_error("Month out of range");

    if (a_day < MIN_DATA_DAY || a_day > daysInMonth(a_month, a_year))
        throw range_error("Day out of range");

    m_year = a_year;
    m_month = a_month;
    m_day = a_day;
}

auto DateModel::day(unsigned short a_day) -> void
{
    if (a_day < MIN_DATA_DAY || a_day > daysInMonth(m_month, m_year))
        throw range_error("Day out of range");

    m_day = a_day;
}
auto DateModel::month(unsigned short a_month) -> void
{
    if (a_month < MIN_DATA_MONTH || a_month > MAX_DATA_MONTH)
        throw range_error("Month out of range");

    if (m_day > daysInMonth(a_month, m_year))
        throw range_error("Current day too big for this month");

    m_month = a_month;
}
auto DateModel::year(unsigned short a_year) -> void
{
    if (a_year < MIN_DATA_YEAR || a_year > MAX_DATA_YEAR)
        throw range_error("Year out of range");

    if (m_day > daysInMonth(m_month, a_year))
        throw range_error("Current day too big for this month in that year");

    m_year = a_year;
}
auto constexpr DateModel::daysInMonth(unsigned short month, unsigned short year) -> unsigned short
{
    switch (month)
    {
    case 1:
        return 31;
    case 2:
        return isLeap(year) ? 29 : 28;
    case 3:
        return 31;
    case 4:
        return 30;
    case 5:
        return 31;
    case 6:
        return 30;
    case 7:
        return 31;
    case 8:
        return 31;
    case 9:
        return 30;
    case 10:
        return 31;
    case 11:
        return 30;
    case 12:
        return 31;
    }
    return 0;
}

auto DateModel::nextDay() -> void
{
    m_day++;

    if (m_day > daysInMonth(m_month, m_year))
    {
        m_day = 1;
        m_month++;

        if (m_month > MAX_DATA_MONTH)
        {
            m_month = MIN_DATA_MONTH;
            m_year++;

            if (m_year > MAX_DATA_YEAR)
            {
                throw range_error("Date out of range");
            }
        }
    }
}

TimeDataModel::TimeDataModel()
    : m_currentDate(MIN_DATA_DAY, MIN_DATA_MONTH, MIN_DATA_YEAR),
      m_currentGameDay(MIN_GAME_DAY),
      m_currentDayOfWeek(DayOfWeek::THURSDAY) // 1st Jan 1942 was a Thursday
{
}

auto TimeDataModel::nextDay() -> void
{
    // 1) następny dzień kalendarza
    m_currentDate.nextDay();

    // 2) przesuwamy dzień tygodnia
    m_currentDayOfWeek = static_cast<DayOfWeek>(
        (static_cast<int>(m_currentDayOfWeek) + 1) % 7);

    // 3) zwiększamy licznik dni gry
    m_currentGameDay++;

    // 4) granica symulacji (opcjonalna)
    if (m_currentGameDay > MAX_GAME_DAY)
    {
        // albo zatrzymaj, albo rzuć wyjątek
        throw std::range_error("Exceeded maximum game days");
    }
    // 5) powiadamiamy obserwatorów
    notifyDayObservers();
}

auto TimeDataModel::nextTeenDays() -> void
{
    for (int i = 0; i < 10; ++i)
    {
        nextDay();
    }
}

auto TimeDataModel::addDayObserver(weak_ptr<DayPassedCallback> a_callback) -> void
{
    m_dayObservers.push_back(move(a_callback));
}

auto TimeDataModel::notifyDayObservers() -> void
{
    for (auto it = m_dayObservers.begin(); it != m_dayObservers.end();)
    {
        if (auto sp = it->lock())
        {
            (*sp)(*this);
            ++it;
        }
        else
        {
            it = m_dayObservers.erase(it);
        }
    }
}