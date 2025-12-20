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
#include <format>

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "Manhattan Project",
        1280, 720,
        SDL_WINDOW_OPENGL);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    // --- IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 150");

    TimeDataModel timeModel; // <-- TWÓJ MODEL
    ResourceConstraints resourceModel; // <-- TWÓJ MODEL
    resourceModel.loadFromJson("./../data/resource_constraints_normal.json");
    ResourcesManager resourcesManager(resourceModel);
    DateHUD dateHUD;
    ResearchHUD researchHUD;
    ResourcesHUD resourcesHUD;

    auto researchManager = std::make_shared<ResearchManager>(timeModel, resourcesManager);
    researchManager->loadFromJson("./../data/technologies.json");

    researchHUD.RegisterListeners(*researchManager);


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

        // New ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // --------------------------------------------------
        // YOUR HUD
        // --------------------------------------------------
        dateHUD.Draw(timeModel);
        researchHUD.Draw(*researchManager);
        researchHUD.DrawTechTree(*researchManager);
        resourcesHUD.Draw(resourcesManager);
        

        // render
        ImGui::Render();
        glViewport(0, 0,
                   (int)io.DisplaySize.x,
                   (int)io.DisplaySize.y);

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    // cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
