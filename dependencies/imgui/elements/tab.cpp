#include "../../../renderer/renderer.hpp"
#include "../../libs/imgui_tricks.hpp"

#include <map>


struct TabStruct {
    float size;
    float hovered;
};


bool renderer::imgui::elements::functions::tab(const char* label, const char* icon, int& tab, int index)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImGuiID id = window->GetID(label);

    std::map<ImGuiID, TabStruct> circle_anim;

    auto it_circle = circle_anim.find(id);
    if (it_circle == circle_anim.end())
    {
        circle_anim.insert({ id, {54, 0} });
        it_circle = circle_anim.find(id);
    }

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImGui::CalcItemSize({ it_circle->second.size, it_circle->second.size }, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    ImRect bb(pos, pos + size);
    ImGui::InvisibleButton(label, size);

    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(7);

    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        tab = index,
        static_vars::current_tab = label;

    it_circle->second.size = 54;
    it_circle->second.hovered = ImTricks::Animations::FastFloatLerp(window->GetID(std::string(label).append("hover").c_str()), index == tab, 0.f, 1.f, 0.0625f);

    window->DrawList->AddRectFilled(bb.Min, bb.Max, ImColor(32, 39, 49), 12);
    window->DrawList->AddRect(bb.Min, bb.Max, ImColor(138, 194, 227, int(it_circle->second.hovered * 255)), 12, 0, 1.2f);

    ImGui::PushClipRect(bb.Min, bb.Max, false);

    window->DrawList->AddText(NULL, 18, bb.Min + ImVec2(it_circle->second.size / 2 - ((ImGui::CalcTextSize(icon).x) / 2) * 1.20,
        it_circle->second.size / 2 - 5),
        ImTricks::Animations::FastColorLerp(
            ImColor(210, 210, 210, 160),
            ImColor(138, 194, 227),
            it_circle->second.hovered), icon);

    ImGui::PopClipRect();

    window->DrawList->AddText(NULL, 14, bb.Min + ImVec2(it_circle->second.size / 2 - (ImGui::CalcTextSize(label).x) / 2, it_circle->second.size + 10), ImTricks::Animations::FastColorLerp(
        ImColor(210, 210, 210, 160),
        ImColor(138, 194, 227),
        it_circle->second.hovered),
        label);
}
