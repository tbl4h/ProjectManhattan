#pragma once
#include <vector>
#include <memory>
#include <functional>

using std::function;
using std::vector;
using std::weak_ptr;

static const constexpr unsigned short MIN_DATA_DAY = 1;
static const constexpr unsigned short MAX_DATA_DAY = 31;
static const constexpr unsigned short MIN_DATA_MONTH = 1;
static const constexpr unsigned short MAX_DATA_MONTH = 12;
static const constexpr unsigned short MIN_DATA_YEAR = 1939;
static const constexpr unsigned short MAX_DATA_YEAR = 1946;
static const constexpr unsigned short MIN_GAME_DAY = 1;
static const constexpr unsigned short MAX_GAME_DAY = 1826;

typedef enum class DayOfWeek
{
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
} DayOfWeek;

class DateModel
{
public:
    DateModel();
    DateModel(unsigned short a_day, unsigned short a_month, unsigned short a_year);
    ~DateModel() = default;

    inline auto day() const -> const unsigned short { return m_day; }
    auto day(unsigned short a_day) -> void;
    inline auto month() const -> const unsigned short { return m_month; }
    auto month(unsigned short a_month) -> void;
    inline auto year() const -> const unsigned short { return m_year; }
    auto year(unsigned short a_year) -> void;
    auto operator<=>(const DateModel &other)const  -> void = default;

    inline auto constexpr isLeap(unsigned short year) -> bool
    { return (year % 4 == 0);}
    auto constexpr daysInMonth(unsigned short month, unsigned short year) -> unsigned short;
    auto nextDay() -> void;

private:
    unsigned short m_day;
    unsigned short m_month;
    unsigned short m_year;
};

class TimeDataModel
{
public:
    using DayPassedCallback = function<void(const TimeDataModel&)>;

    TimeDataModel();
    ~TimeDataModel() = default;

    inline auto currentDate() const -> const DateModel & { return m_currentDate; }
    inline auto currentGameDay() const -> const unsigned short { return m_currentGameDay; }
    inline auto currentDayOfWeek() const -> const DayOfWeek { return m_currentDayOfWeek; }
    
    auto nextDay() -> void;
    auto nextTeenDays() -> void;
    auto addDayObserver(weak_ptr<DayPassedCallback> a_callback) -> void;
    
private:
    auto notifyDayObservers() -> void;

private:
    DateModel m_currentDate;
    unsigned short m_currentGameDay;
    DayOfWeek m_currentDayOfWeek;
    vector<weak_ptr<DayPassedCallback>> m_dayObservers;
};
