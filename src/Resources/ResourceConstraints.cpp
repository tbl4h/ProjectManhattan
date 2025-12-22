#pragma once
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "ResourceConstraints.hpp"

using std::ifstream;
using std::cerr;
using json = nlohmann::json;


bool ResourceConstraints::loadFromJson(const string &path)
{
    ifstream file(path);
    if (!file.is_open())
    {
        cerr << "Failed to open resource constraints file: " << path << "\n";
        return false;
    }

    json data;
    try
    {
        file >> data;
    }
    catch (const json::parse_error &e)
    {
        cerr << "JSON parse error: " << e.what() << "\n";
        return false;
    }

    try
    {
        // money
        auto &m = data["money"];
        real_budget = m["real_budget"];
        minimum_budget = m["minimum_budget"];
        maximum_budget = m["maximum_budget"];

        preliminary_minimum = m["preliminary_minimum"];
        preliminary_maximum = m["preliminary_maximum"];

        infrastructure_minimum = m["infrastructure_minimum"];
        infrastructure_maximum = m["infrastructure_maximum"];

        full_scale_minimum = m["full_scale_minimum"];
        full_scale_maximum = m["full_scale_maximum"];

        fussed_fuel_minimum = m["fussed_fuel_minimum"];
        fussed_fuel_maximum = m["fussed_fuel_maximum"];

        bomb_assembly_minimum = m["bomb_assembly_minimum"];
        bomb_assembly_maximum = m["bomb_assembly_maximum"];

        wind_down_minimum = m["wind_down_minimum"];
        wind_down_maximum = m["wind_down_maximum"];

        initial_money = m["initial_money"];
        initial_uranium = m["initial_uranium"];
        initial_plutonium = m["initial_plutonium"];
        maximal_uranium = m["maximal_uranium"];
        maximal_plutonium = m["maximal_plutonium"];
        initial_morale = m["initial_morale"];
        initial_security = m["initial_security"];
        minimal_total_morale = m["minimal_total_morale"];
        maximal_total_morale = m["maximal_total_morale"];
        minimal_total_security = m["minimal_total_security"];
        maximal_total_security = m["maximal_total_security"];

        // personnel
        auto &p = data["personnel"];
        worker_daily_cost = p["worker_daily_cost"];
        worker_hiring_cost = p["worker_hiring_cost"];
        scientist_daily_cost = p["scientist_daily_cost"];
        scientist_hiring_cost = p["scientist_hiring_cost"];
        engineer_daily_cost = p["engineer_daily_cost"];
        engineer_hiring_cost = p["engineer_hiring_cost"];
        army_personnel_daily_cost = p["army_personnel_daily_cost"];
        army_personnel_hiring_cost = p["army_personnel_hiring_cost"];

        total_numbers_of_all_personnel = p["total_numbers_of_all_personnel"];
        initial_total_workers = p["initial_total_workers"];
        maximum_total_workers = p["maximum_total_workers"];
        initial_total_scientists = p["initial_total_scientists"];
        maximum_total_scientists = p["maximum_total_scientists"];
        initial_total_engineers = p["initial_total_engineers"];
        maximum_total_engineers = p["maximum_total_engineers"];
        initial_total_army_personnel = p["initial_total_army_personnel"];
        maximum_total_army_personnel = p["maximum_total_army_personnel"];
    }
    catch (const json::exception &e)
    {
        std::cerr << "JSON field error: " << e.what() << "\n";
        return false;
    }

    return true;
}
