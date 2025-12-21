#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

#include "Research/ResearchManager.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Core/header/TimeSystem.hpp"

using namespace std;

namespace fs = std::filesystem;

class ResearchManagerTest : public ::testing::Test
{
protected:
    TimeDataModel timeModel;
    ResourceConstraints constraints;
    ResourcesManager resources;
    ResearchManager research;

    fs::path jsonPath;

    ResearchManagerTest()
        : resources(constraints,timeModel),
          research(timeModel, resources)
    {
        // --- przygotuj zasoby ---
        resources.addMoney(100000); // wystarczająco na testy
        resources.setTotalScientists(100);
        resources.hireScientists(100);

        // --- wygeneruj JSON testowy ---
        jsonPath = fs::temp_directory_path() / "test_technologies.json";

        ofstream file(jsonPath);
        file << R"(
{
    "technologies": [
        {
            "id": "basic_physics",
            "name": "Basics of Nuclear Physics",
            "type": "theory",
            "research_days": 3,
            "prerequisites": [],
            "description": "Understanding the basic principles of nuclear physics.",
            "money_cost": 1000,
            "dayly_cost": 100,
            "uranium_required": 0,
            "plutonium_required": 0,
            "workers_required": 0,
            "engineers_required": 0,
            "scientists_required": 10,
            "army_personnel_required": 0,
            "building_required": [],
            "characters_involved": []
        },
        {
            "id": "uranium_enrichment",
            "name": "Uranium Enrichment",
            "type": "theory",
            "research_days": 5,
            "prerequisites": ["basic_physics"],
            "description": "Separating U-235 from natural uranium.",
            "money_cost": 2000,
            "dayly_cost": 200,
            "uranium_required": 0,
            "plutonium_required": 0,
            "workers_required": 0,
            "engineers_required": 0,
            "scientists_required": 20,
            "army_personnel_required": 0,
            "building_required": [],
            "characters_involved": []
        }
    ]
}
)";
        file.close();

        research.loadFromJson(jsonPath.string());
    }

    ~ResearchManagerTest() override
    {
        if (fs::exists(jsonPath))
            fs::remove(jsonPath);
    }
};

/* -------------------------------------------------- */

TEST_F(ResearchManagerTest, LoadsTechnologies)
{
    EXPECT_EQ(research.getAllTechnologies().size(), 2);
}

TEST_F(ResearchManagerTest, RootTechnologyIsAvailable)
{
    EXPECT_TRUE(research.isAvailable("basic_physics"));
}

TEST_F(ResearchManagerTest, StartResearchSucceeds)
{
    EXPECT_TRUE(research.startResearch("basic_physics"));
    EXPECT_NE(research.getActiveResearch(), nullptr);
}

TEST_F(ResearchManagerTest, ResearchProgressIncreasesWithDays)
{
    research.startResearch("basic_physics");

    timeModel.nextDay();
    timeModel.nextDay();

    EXPECT_GT(research.getProgress("basic_physics"), 0.0f);
}

TEST_F(ResearchManagerTest, ResearchCompletesAfterEnoughDays)
{
    research.startResearch("basic_physics");

    timeModel.nextDay();
    timeModel.nextDay();
    timeModel.nextDay();

    EXPECT_TRUE(research.isCompleted("basic_physics"));
    EXPECT_EQ(research.getActiveResearch(), nullptr);
}

TEST_F(ResearchManagerTest, UnlocksDependentTechnology)
{
    research.startResearch("basic_physics");

    for (int i = 0; i < 3; ++i)
        timeModel.nextDay();

    EXPECT_TRUE(research.isAvailable("uranium_enrichment"));
}

TEST_F(ResearchManagerTest, ResearchCompletedCallbackIsCalled)
{
    bool called = false;

    research.addResearchCompletedListener(
        [&](const Technology &tech)
        {
            called = true;
            EXPECT_EQ(tech.m_id, "basic_physics");
        });

    research.startResearch("basic_physics");

    for (int i = 0; i < 3; ++i)
        timeModel.nextDay();

    EXPECT_TRUE(called);
}

TEST_F(ResearchManagerTest, CannotStartResearchWithoutMoney)
{
    ResourcesManager poorResources(constraints,timeModel);
    ResearchManager poorResearch(timeModel, poorResources);

    poorResearch.loadFromJson(jsonPath.string());

    EXPECT_FALSE(poorResearch.startResearch("basic_physics"));
}

TEST_F(ResearchManagerTest, MissingResourcesCallbackIsCalled)
{
    bool called = false;

    resources.fireScientists(100); // brak naukowców

    research.addResearchMissingResourcesListener(
        [&](const ResourceMissing &, const Technology &tech)
        {
            called = true;
            EXPECT_EQ(tech.m_id, "basic_physics");
        });

    EXPECT_FALSE(research.startResearch("basic_physics"));
    EXPECT_TRUE(called);
}

TEST_F(ResearchManagerTest, GetActiveResearchReturnsNullWhenIdle)
{
    EXPECT_EQ(research.getActiveResearch(), nullptr);
}
