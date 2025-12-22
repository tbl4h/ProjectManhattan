#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include "Core/header/TimeSystem.hpp"
#include "Research/ResearchManager.hpp"
#include "Resources/ResourcesManager.hpp"

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_opengl3.h"

// UI
#include "UI/UIVisibility.hpp"
#include "UI/UISync.hpp"
#include "UI/TopBarHUD.hpp"
#include "UI/DateHUD.hpp"
#include "UI/ResourcesHUD.hpp"

// Research MVC
#include "UI/ResearchHUDController.hpp"
#include "UI/ResearchListHUD.hpp"
#include "UI/TechTreeHUD.hpp"

int main()
{
    // =======================
    // SDL + OpenGL
    // =======================
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "Manhattan Project",
        1280, 720,
        SDL_WINDOW_OPENGL);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    // =======================
    // IMGUI
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

    ResourceConstraints resourceConstraints;
    resourceConstraints.loadFromJson(
        "./../data/resource_constraints_normal.json");

    ResourcesManager resourcesManager(
        resourceConstraints,
        timeModel);

    ResearchManager researchManager(
        timeModel,
        resourcesManager);

    researchManager.loadFromJson(
        "./../data/technologies.json");

    // =======================
    // UI STATE
    // =======================
    UIVisibility ui;

    // =======================
    // HUDs (Views)
    // =======================
    TopBarHUD topBarHUD;
    DateHUD dateHUD;
    ResourcesHUD resourcesHUD;

    // =======================
    // RESEARCH MVC
    // =======================
    ResearchHUDController researchController(researchManager);

    ResearchListHUD researchListHUD(researchController);
    TechTreeHUD techTreeHUD(researchController);

    // brak surowców → ResourcesHUD
    researchManager.addResearchMissingResourcesListener(
        [&](const ResourceMissing &missing)
        {
            resourcesHUD.OnResearchMissingResources(missing);
        });

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

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // =======================
        // DRAW HUDs
        // =======================
        topBarHUD.Draw(ui);

        // 🔁 Synchronizacja widoczności (JEDYNE MIEJSCE)
        SyncVisibility(ui.showDate, ui.lastDate, dateHUD);
        SyncVisibility(ui.showResearch, ui.lastResearch, researchListHUD);
        SyncVisibility(ui.showTechTree, ui.lastTechTree, techTreeHUD);
        SyncVisibility(ui.showResources, ui.lastResources, resourcesHUD);

        // =======================
        // Rysowanie
        // =======================
        if (ui.showDate)
            dateHUD.Draw(timeModel);

        if (ui.showResearch)
            researchListHUD.Draw();

        if (ui.showTechTree)
            techTreeHUD.Draw();

        if (ui.showResources)
            resourcesHUD.Draw(resourcesManager);

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
