#include <gtest/gtest.h>
#include "./../../../src/Core/header/TimeSystem.hpp"

TEST(DateModelTests, LeapYearFebruary) {
    DateModel d{28, 2, 1944};   // 1944 = leap
    d.nextDay();
    EXPECT_EQ(d.day(), 29);
    EXPECT_EQ(d.month(), 2);
}

TEST(DateModelTests, EndOfMonth) {
    DateModel d{31, 1, 1942};
    d.nextDay();
    EXPECT_EQ(d.day(), 1);
    EXPECT_EQ(d.month(), 2);
}

TEST(DateModelTests, EndOfYear) {
    DateModel d{31, 12, 1945};
    d.nextDay();
    EXPECT_EQ(d.day(), 1);
    EXPECT_EQ(d.month(), 1);
    EXPECT_EQ(d.year(), 1946);
}

TEST(DateModelTests, OutOfRange) {
    EXPECT_THROW(DateModel(1, 1, 1947), std::range_error);
    EXPECT_THROW(DateModel(32, 1, 1942), std::range_error);
}

TEST(TimeDataModelTests, BasicNextDay) {
    TimeDataModel t;
    auto startDay = t.currentGameDay();

    t.nextDay();
    EXPECT_EQ(t.currentGameDay(), startDay + 1);
}

TEST(TimeDataModelTests, DayOfWeekRotate) {
    TimeDataModel t;

    DayOfWeek start = t.currentDayOfWeek();
    t.nextDay();

    EXPECT_EQ(
        t.currentDayOfWeek(),
        static_cast<DayOfWeek>((static_cast<int>(start) + 1) % 7)
    );
}

TEST(TimeDataModelTests, ObserverCalled) {
    TimeDataModel t;

    bool called = false;

    auto cb = std::make_shared<TimeDataModel::DayPassedCallback>(
        [&](const TimeDataModel&){
            called = true;
        }
    );

    t.addDayObserver(cb);
    t.nextDay();

    EXPECT_TRUE(called);
}

TEST(TimeDataModelTests, DeadObserverDoesNotCrash) {
    TimeDataModel t;

    {
        auto cb = std::make_shared<TimeDataModel::DayPassedCallback>(
            [&](const TimeDataModel&){}
        );
        t.addDayObserver(cb);
    } // cb destroyed here

    EXPECT_NO_THROW(t.nextDay());
}

TEST(TimeDataModelTests, MaxGameDayThrows) {
    TimeDataModel t;

    t = TimeDataModel();

    // jump near MAX_GAME_DAY for speed
    for(int i = 0; i < MAX_GAME_DAY - 1; ++i)
        t.nextDay();

    EXPECT_THROW(t.nextDay(), std::range_error);
}

TEST(TimeDataModelTests, MaxDateThrows) {
    TimeDataModel t;

    // ustaw koniec zakresu
    while(true) {
        try { t.nextDay(); }
        catch(...) { break; }
    }

    EXPECT_THROW(t.nextDay(), std::range_error);
}
