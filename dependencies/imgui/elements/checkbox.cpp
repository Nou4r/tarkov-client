#include "../../../renderer/renderer.hpp"

#include "../../libs/imgui_tricks.hpp"

void renderer::imgui::elements::functions::checkbox(const char* label, const char* desc, bool& v)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    int flags = 0;
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImGui::CalcItemSize({ 328, 57 }, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

    if (hovered)
        ImGui::SetMouseCursor(7);

    if (pressed)
        v = !v;

    float Active = ImTricks::Animations::FastFloatLerp(id, v == true, 0, 21, 2);

    window->DrawList->AddRectFilled(bb.Min + ImVec2(328 - 40, 22), bb.Min + ImVec2(328, 34), ImColor(32, 39, 49), 12);

    window->DrawList->AddCircleFilled(bb.Min + ImVec2(328 + 9 - 40 + Active, 25 + 3), 10, ImColor(19, 19, 19), 32);
    window->DrawList->AddCircleFilled(bb.Min + ImVec2(328 + 9 - 40 + Active, 25 + 3), 7, ImTricks::Animations::FastColorLerp(ImColor(211, 211, 211, 211), ImColor(138, 194, 227), Active / 18.f), 32);

    window->DrawList->AddText(fonts::px18, 18, bb.Min + ImVec2(0, 12), ImColor(228, 228, 230), label);
    window->DrawList->AddText(bb.Min + ImVec2(0, 31), ImColor(156, 167, 190, 125), desc);
}
