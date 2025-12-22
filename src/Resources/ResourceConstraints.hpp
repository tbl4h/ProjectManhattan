#pragma once
#include <string>

using std::string;

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
