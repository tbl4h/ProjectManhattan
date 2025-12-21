#include "DateHUD.hpp"
#include "imgui.h"

void DateHUD::Draw(TimeDataModel &time)
{
    const auto &date = time.currentDate();

    auto &io = ImGui::GetIO();

    m_position = ImVec2(io.DisplaySize.x - m_windowSize.x, 0);

    ImGui::SetNextWindowPos(m_position);
    ImGui::SetNextWindowSize(m_windowSize);

    if (!m_visible)
        return;

    bool open = true;

    ImGui::Begin("DateHUD", nullptr, m_flags);

    if (!open)
    {
        m_visible = false;
        ImGui::End();
        return;
    }

    ImGui::Text("Date: %02d.%02d.%04d",
                date.day(), date.month(), date.year());

    ImGui::Text("Day Of Week: %d",
                (int)time.currentDayOfWeek() + 1);

    ImGui::Text("Game day: %d", time.currentGameDay());

    if (ImGui::Button("Next day"))
    {
        time.nextDay();
    }

    if (ImGui::Button("Next 10 days"))
    {
        time.nextTeenDays();
    }

    ImGui::End();
}
