#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include "Core/header/TimeSystem.hpp"
#include "Research/ResearchManager.hpp"
#include "Resources/ResourcesManager.hpp"

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_opengl3.h"

#include "UI/DateHUD.hpp"
#include "UI/ResearchHUD.hpp"
#include "UI/ResourcesHUD.hpp"
#include "UI/UIVisibility.hpp"
#include "UI/UISync.hpp"
#include "UI/TopBarHUD.hpp"
#include "UI/ResearchListHUD.hpp"
#include "UI/TechTreeHUD.hpp"

#include <format>

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "Manhattan Project",
        1280, 720,
        SDL_WINDOW_OPENGL);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    // =======================
    // IMGUI SETUP
    // =======================
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 150");

    // =======================
    // CORE MODELS
    // =======================
    TimeDataModel timeModel;

    ResourceConstraints resourceModel;
    resourceModel.loadFromJson(
        "./../data/resource_constraints_normal.json");

    ResourcesManager resourcesManager(resourceModel, timeModel);

    // =======================
    // HUDs
    // =======================
    UIVisibility ui;
    TopBarHUD topBarHUD;
    DateHUD dateHUD;
    ResearchHUDController researchHUDController();
    
    ResearchListHUD researchListHUD(researchHUDController);
    ResourcesHUD resourcesHUD;

    // =======================
    // RESEARCH SYSTEM
    // =======================
    ResearchManager researchManager(
        timeModel,
        resourcesManager);

    researchManager.loadFromJson(
        "./../data/technologies.json");

    // 🔴 KLUCZOWE POŁĄCZENIE: brak surowców → ResourcesHUD
    researchManager.addResearchMissingResourcesListener(
        [&](const ResourceMissing &missing,
            const Technology &tech)
        {
            resourcesHUD.OnResearchMissingResources(
                missing,
                tech);
        });

    // standardowe listenery ResearchHUD (completed, itp.)
    researchHUD.RegisterListeners(researchManager);

    // =======================
    // MAIN LOOP
    // =======================
    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);

            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        // ---- ImGui frame ----
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // =======================
        // TOP BAR
        // =======================
        topBarHUD.Draw(ui);

        // =======================
        // SYNC VISIBILITY
        // =======================
        SyncVisibility(ui.showDate, ui.lastDate, dateHUD);
        SyncVisibility(ui.showResearch, ui.lastResearch, researchHUD);
        SyncVisibility(ui.showResources, ui.lastResources, resourcesHUD);

        // TechTree jeśli osobna flaga:
        researchHUD.SetVisibleTechTree(ui.showTechTree);
        ui.showTechTree = researchHUD.IsTechTreeVisible();

        // =======================
        // DRAW HUDs
        // =======================
        if (ui.showDate)
            dateHUD.Draw(timeModel);
        if (ui.showResearch)
            researchHUD.Draw(researchManager);
        if (ui.showTechTree)
            researchHUD.DrawTechTree(researchManager);
        if (ui.showResources)
            resourcesHUD.Draw(resourcesManager);

        // --- Research window ---
        // static bool lastResearch = ui.showResearch;
        // if (ui.showResearch != lastResearch)
        // {
        //     researchHUD.SetVisible(ui.showResearch);
        //     lastResearch = ui.showResearch;
        // }

        // // --- Tech tree ---
        // static bool lastTechTree = ui.showTechTree;
        // if (ui.showTechTree != lastTechTree)
        // {
        //     researchHUD.SetVisibleTechTree(ui.showTechTree);
        //     lastTechTree = ui.showTechTree;
        // }

        // // --- Resources ---
        // static bool lastResources = ui.showResources;
        // if (ui.showResources != lastResources)
        // {
        //     resourcesHUD.SetVisible(ui.showResources);
        //     lastResources = ui.showResources;
        // }

        // if (ui.showDate)
        //     dateHUD.Draw(timeModel);

        // if (ui.showResearch)
        //     researchHUD.Draw(researchManager);

        // if (ui.showTechTree)
        //     researchHUD.DrawTechTree(researchManager);

        // if (ui.showResources)
        //     resourcesHUD.Draw(resourcesManager);

        // ui.showResources = resourcesHUD.IsVisible();
        // ui.showResearch = researchHUD.IsVisible();
        // ui.showTechTree = researchHUD.IsTechTreeVisible();

        // =======================
        // RENDER
        // =======================
        ImGui::Render();

        glViewport(
            0, 0,
            (int)io.DisplaySize.x,
            (int)io.DisplaySize.y);

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(
            ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    // =======================
    // CLEANUP
    // =======================
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
