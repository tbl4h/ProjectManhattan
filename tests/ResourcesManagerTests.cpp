#include <gtest/gtest.h>

#include "Resources/ResourcesManager.hpp"
#include "Core/header/TimeSystem.hpp"

class ResourcesManagerTest : public ::testing::Test
{
protected:
    TimeDataModel time;
    ResourceConstraints constraints;
    ResourcesManager resources;

    ResourcesManagerTest()
        : resources(constraints, time)
    {
        // Zapewniamy sensowne dane startowe
        constraints.initial_money = 1000;
        constraints.initial_morale = 50;
        constraints.initial_security = 50;
    }

    ~ResourcesManagerTest() noexcept override = default;

};

/* ============================================================
 *  spendMoney — BEZ ZEJŚCIA PONIŻEJ ZERA
 * ============================================================ */

TEST_F(ResourcesManagerTest, SpendMoneySucceedsWhenEnoughMoney)
{
    long before = resources.getMoney();
    EXPECT_TRUE(resources.spendMoney(500));
    EXPECT_EQ(resources.getMoney(), before - 500);
}

TEST_F(ResourcesManagerTest, SpendMoneyFailsWhenNotEnoughMoney)
{
    long before = resources.getMoney();
    EXPECT_FALSE(resources.spendMoney(before + 1));
    EXPECT_EQ(resources.getMoney(), before); // brak zmiany
}

/* ============================================================
 *  onDayPassed — MOŻE ZROBIĆ UJEMNE SALDO
 * ============================================================ */

TEST_F(ResourcesManagerTest, DailyCostIsSubtractedOnDayPassed)
{
    resources.hireWorkers(10); // koszt dzienny > 0
    long before = resources.getMoney();

    time.nextDay();

    EXPECT_LT(resources.getMoney(), before);
}

TEST_F(ResourcesManagerTest, MoneyCanBecomeNegativeAfterDayPassed)
{
    resources.hireScientists(100); // duży koszt
    resources.spendMoney(resources.getMoney() - 10); // zostaje mało

    time.nextDay();

    EXPECT_LT(resources.getMoney(), 0);
}

/* ============================================================
 *  MORALE I SECURITY — TYLKO W onDayPassed
 * ============================================================ */

TEST_F(ResourcesManagerTest, MoraleAndSecurityDecreaseWhenMoneyIsNegative)
{
    resources.hireScientists(100);
    resources.spendMoney(resources.getMoney() - 1);

    unsigned moraleBefore = resources.getMorale();
    unsigned securityBefore = resources.getSecurity();

    time.nextDay();

    EXPECT_EQ(resources.getMorale(), moraleBefore - 2);
    EXPECT_EQ(resources.getSecurity(), securityBefore - 1);
}

TEST_F(ResourcesManagerTest, MoraleIncreasesWhenMoneyIsPositive)
{
    unsigned moraleBefore = resources.getMorale();

    time.nextDay();

    EXPECT_EQ(resources.getMorale(), moraleBefore + 1);
}

/* ============================================================
 *  DAILY RESET
 * ============================================================ */

TEST_F(ResourcesManagerTest, DailyHiredCountersAreReset)
{
    resources.hireWorkers(5);
    resources.hireScientists(3);

    time.nextDay(); // reset

    // Brak getterów → test pośredni:
    EXPECT_TRUE(resources.hireWorkers(1));
    EXPECT_TRUE(resources.hireScientists(1));
}

/* ============================================================
 *  CLAMPY MORALE / SECURITY
 * ============================================================ */

TEST_F(ResourcesManagerTest, MoraleIsClampedToLimits)
{
    resources.addMorale(1000);
    EXPECT_EQ(resources.getMorale(), constraints.maximal_total_morale);

    resources.reduceMorale(1000);
    EXPECT_EQ(resources.getMorale(), constraints.minimal_total_morale);
}

TEST_F(ResourcesManagerTest, SecurityIsClampedToLimits)
{
    resources.addSecurity(1000);
    EXPECT_EQ(resources.getSecurity(), constraints.maximal_total_security);

    resources.reduceSecurity(1000);
    EXPECT_EQ(resources.getSecurity(), constraints.minimal_total_security);
}
