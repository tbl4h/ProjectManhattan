cmake_minimum_required(VERSION 3.16)
project(ManhattanProject LANGUAGES CXX)

# =========================================================
# C++ STANDARD
# =========================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


# =========================================================
# SYSTEM LIBRARIES
# =========================================================
find_package(OpenGL REQUIRED)
find_package(SDL3 REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(GTest REQUIRED)

# =========================================================
# IMGUI (vendorowane w repo)
# =========================================================
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/third_party/imgui)

file(GLOB IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(ImGui STATIC ${IMGUI_SOURCES})

target_include_directories(ImGui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_link_libraries(ImGui PUBLIC SDL3::SDL3 OpenGL::GL)


# =========================================================
# MAIN GAME
# =========================================================
add_executable(Manhattan
    src/main.cpp

    src/Core/header/TimeSystem.hpp
    src/Core/src/TimeSystem.cpp

    src/UI/DateHUD.hpp
    src/UI/DateHUD.cpp
    src/UI/ResearchHUD/ResearchHUD.hpp
    src/UI/ResearchHUD/ResearchHUD.cpp
    src/UI/ResourcesHUD.hpp
    src/UI/ResourcesHUD.cpp
    src/UI/TopBarHUD.hpp
    src/UI/TopBarHUD.cpp
    src/UI/UIVisibility.hpp
    src/UI/IHUD.hpp
    src/UI/UISync.hpp
    src/UI/ResearchHUD/ResearchHUDController.hpp
    src/UI/ResearchHUD/ResearchHUDController.cpp
    src/UI/ResearchHUD/ResearchListHUD.hpp
    src/UI/ResearchHUD/ResearchListHUD.cpp
    src/UI/ResearchHUD/TechTreeHUD.hpp
    src/UI/ResearchHUD/TechTreeHUD.cpp
    src/UI/ResearchHUD/ResearchCompletedPopupHUD.hpp
    src/UI/ResearchHUD/ResearchCompletedPopupHUD.cpp

    src/Research/ResearchManager.hpp
    src/Research/ResearchManager.cpp

    src/Resources/ResourcesManager.hpp
    src/Resources/ResourcesManager.cpp
    src/Resources/ResourceConstraints.hpp
    src/Resources/ResourceConstraints.cpp 
    src/Resources/ResourceMissing.hpp 
)

target_include_directories(Manhattan PRIVATE src)

target_link_libraries(Manhattan PRIVATE
    SDL3::SDL3
    ImGui
    OpenGL::GL
    nlohmann_json::nlohmann_json
)

set_target_properties(Manhattan PROPERTIES
    BUILD_RPATH "/usr/lib/x86_64-linux-gnu"
)


# =========================================================
# TESTS
# =========================================================
enable_testing()

file(GLOB_RECURSE TEST_SOURCES tests/*.cpp tests/*.hpp)

add_executable(ManhattanTests
    ${TEST_SOURCES}
    src/Core/header/TimeSystem.hpp
    src/Core/src/TimeSystem.cpp
    src/Research/ResearchManager.hpp
    src/Research/ResearchManager.cpp
    src/Resources/ResourcesManager.hpp
    src/Resources/ResourcesManager.cpp

)

target_include_directories(ManhattanTests PRIVATE src)

target_link_libraries(ManhattanTests PRIVATE
    GTest::gtest
    GTest::gtest_main
    nlohmann_json::nlohmann_json
)

set_target_properties(ManhattanTests PROPERTIES
    BUILD_RPATH "/usr/lib/x86_64-linux-gnu"
)


include(GoogleTest)
gtest_discover_tests(ManhattanTests)
