#pragma once

struct UIVisibility
{
    bool showDate       = true;
    bool showResearch   = true;
    bool showTechTree   = true;
    bool showResources  = true;

    // stany poprzednie (do detekcji zmian)
    bool lastDate       = showDate;
    bool lastResearch   = showResearch;
    bool lastTechTree   = showTechTree;
    bool lastResources  = showResources;
};
