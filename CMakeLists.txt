cmake_minimum_required(VERSION 3.16)
project(ManhattanProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(ExternalProject)
include(FetchContent)

set(DEPS_SRC_DIR     ${CMAKE_SOURCE_DIR}/deps/src)
set(DEPS_BUILD_DIR   ${CMAKE_SOURCE_DIR}/deps/build)
set(DEPS_INSTALL_DIR ${CMAKE_SOURCE_DIR}/deps/lib)
set(DEPS_MARKER_FILE ${CMAKE_SOURCE_DIR}/deps/.installed)

file(MAKE_DIRECTORY ${DEPS_INSTALL_DIR}/include)
file(MAKE_DIRECTORY ${DEPS_INSTALL_DIR}/lib)

#############################################################
### SDL3 EXTERNAL PROJECT
#############################################################

if(NOT EXISTS ${DEPS_MARKER_FILE})

    message(STATUS "Building SDL3")
    
    ExternalProject_Add(SDL3_ext
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.4

        SOURCE_DIR     ${DEPS_SRC_DIR}/sdl3
        BINARY_DIR     ${DEPS_BUILD_DIR}/sdl3
        INSTALL_DIR    ${DEPS_INSTALL_DIR}

        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
            -DSDL_SHARED=ON
    )

    add_custom_target(deps_marker ALL
        COMMAND ${CMAKE_COMMAND} -E touch ${DEPS_MARKER_FILE}
        DEPENDS SDL3_ext
    )

else()
    message(STATUS "SDL3 already installed — skipping")
    add_custom_target(SDL3_ext)
endif()

#############################################################
### IMPORT SDL3
#############################################################

add_library(SDL3 SHARED IMPORTED GLOBAL)
set_target_properties(SDL3 PROPERTIES
    IMPORTED_LOCATION ${DEPS_INSTALL_DIR}/lib/libSDL3.so
)
target_include_directories(SDL3 INTERFACE ${DEPS_INSTALL_DIR}/include)

#############################################################
### IMGUI (as sources inside project)
#############################################################

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG master
)
FetchContent_MakeAvailable(imgui)

file(GLOB IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/*.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(ImGui STATIC ${IMGUI_SOURCES})

target_include_directories(ImGui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(ImGui PUBLIC SDL3)

find_package(OpenGL REQUIRED)

#############################################################
### nlohmann/json (Header-only)
#############################################################

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(nlohmann_json)

# Biblioteka dostępna jako:
#   nlohmann_json::nlohmann_json

#############################################################
### MAIN GAME
#############################################################

add_executable(Manhattan
    src/main.cpp
    src/Core/header/TimeSystem.hpp
    src/Core/src/TimeSystem.cpp
    src/UI/DateHUD.hpp
    src/UI/DateHUD.cpp
    src/UI/ResearchHUD.hpp
    src/UI/ResearchHUD.cpp
    src/UI/ResourcesHUD.hpp
    src/UI/ResourcesHUD.cpp
    src/Research/ResearchManager.hpp
    src/Research/ResearchManager.cpp
    src/Resources/ResourcesManager.hpp
    src/Resources/ResourcesManager.cpp
)

target_include_directories(Manhattan PRIVATE src)

target_link_libraries(Manhattan PRIVATE
    SDL3
    ImGui
    OpenGL::GL
    nlohmann_json::nlohmann_json
)

#############################################################
### TESTS
#############################################################

enable_testing()
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    message(STATUS "GTest not found globally — fetching")

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.14.0
    )
    FetchContent_MakeAvailable(googletest)
endif()

file(GLOB_RECURSE TEST_SOURCES tests/*.cpp tests/*.hpp)

add_executable(ManhattanTests
    ${TEST_SOURCES}
    src/Core/header/TimeSystem.hpp
    src/Core/src/TimeSystem.cpp
)

target_include_directories(ManhattanTests PRIVATE src)

target_link_libraries(ManhattanTests PRIVATE
    GTest::gtest
    GTest::gtest_main
    nlohmann_json::nlohmann_json
)

include(GoogleTest)
gtest_discover_tests(ManhattanTests)
