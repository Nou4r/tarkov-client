#include "../../../renderer/renderer.hpp"

#include "../../libs/imgui_tricks.hpp"

#include <map>

void renderer::imgui::elements::functions::color_picker(const char* identifier, float* col)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    int flags = 0;
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(identifier);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImGui::CalcItemSize({ 328, 18 + 10 }, 318, 18 + 5);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return;

    // RED
    window->DrawList->AddCircleFilled(bb.Min + ImVec2(9, 9), 7, ImColor(176, 75, 81), 32);

    if (col[0] == 176 / 255.f && col[1] == 75 / 255.f && col[2] == 81 / 255.f)
        window->DrawList->AddCircle(bb.Min + ImVec2(9, 9), 9, ImColor(211, 211, 211), 32);

    if (ImGui::IsMouseHoveringRect(bb.Min + ImVec2(1, 0), bb.Min + ImVec2(18, 18)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        col[0] = 176 / 255.f; col[1] = 75 / 255.f; col[2] = 81 / 255.f; col[3] = 255 / 255.f;
    }


    // Purple
    window->DrawList->AddCircleFilled(bb.Min + ImVec2(30, 9), 7, ImColor(127, 128, 246));

    if (col[0] == 127 / 255.f && col[1] == 128 / 255.f && col[2] == 246 / 255.f)
        window->DrawList->AddCircle(bb.Min + ImVec2(30, 9), 9, ImColor(211, 211, 211), 32);

    if (ImGui::IsMouseHoveringRect(bb.Min + ImVec2(21, 0), bb.Min + ImVec2(39, 18)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        col[0] = 127 / 255.f; col[1] = 128 / 255.f; col[2] = 246 / 255.f; col[3] = 255 / 255.f;
    }

    // Light Green
    window->DrawList->AddCircleFilled(bb.Min + ImVec2(51, 9), 7, ImColor(211, 243, 107), 32);

    if (col[0] == 211 / 255.f && col[1] == 243 / 255.f && col[2] == 107 / 255.f)
        window->DrawList->AddCircle(bb.Min + ImVec2(51, 9), 9, ImColor(211, 211, 211), 32);

    if (ImGui::IsMouseHoveringRect(bb.Min + ImVec2(42, 0), bb.Min + ImVec2(60, 18)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        col[0] = 211 / 255.f; col[1] = 243 / 255.f; col[2] = 107 / 255.f; col[3] = 255 / 255.f;
    }

    // Orange
    window->DrawList->AddCircleFilled(bb.Min + ImVec2(72, 9), 7, ImColor(255, 199, 97), 32);

    if (col[0] == 255 / 255.f && col[1] == 199 / 255.f && col[2] == 97 / 255.f)
        window->DrawList->AddCircle(bb.Min + ImVec2(72, 9), 9, ImColor(211, 211, 211), 32);

    if (ImGui::IsMouseHoveringRect(bb.Min + ImVec2(63, 0), bb.Min + ImVec2(81, 18)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        col[0] = 255 / 255.f; col[1] = 199 / 255.f; col[2] = 97 / 255.f; col[3] = 255 / 255.f;
    }

    // Green Blue
    window->DrawList->AddCircleFilled(bb.Min + ImVec2(93, 9), 7, ImColor(47, 219, 188), 32);

    if (col[0] == 47 / 255.f && col[1] == 219 / 255.f && col[2] == 188 / 255.f)
        window->DrawList->AddCircle(bb.Min + ImVec2(93, 9), 9, ImColor(211, 211, 211), 32);

    if (ImGui::IsMouseHoveringRect(bb.Min + ImVec2(84, 0), bb.Min + ImVec2(102, 18)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        col[0] = 47 / 255.f; col[1] = 219 / 255.f; col[2] = 188 / 255.f; col[3] = 255 / 255.f;
    }

    std::map<ImGuiID, bool> circle_anim;

    auto it_circle = circle_anim.find(id);
    if (it_circle == circle_anim.end())
    {
        circle_anim.insert({ id, false });
        it_circle = circle_anim.find(id);
    }

    ImGui::SetNextWindowSize({ 245.f,  200.f });
    if (ImGui::BeginPopup(identifier, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar))
    {
        ImGui::SetCursorPos({ 10,10 });
        ImGui::BeginGroup();
        {
            ImGui::ColorPicker4("##coloredit", col, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);
        }
        ImGui::EndGroup();

        if (!ImGui::IsWindowHovered())
            it_circle->second = false;

        ImGui::EndPopup();
    }

    ImGui::RenderColorRectWithAlphaCheckerboard(window->DrawList, bb.Max - ImVec2(16, 28), bb.Max - ImVec2(0, 12), ImColor(col[0], col[1], col[2], col[3]), 3.5f, ImVec2(0, 0), 18);
    window->DrawList->AddText(bb.Max - ImVec2(20 + ImGui::CalcTextSize("Custom").x, 28), ImColor(211, 211, 211), "Custom");

    if (ImGui::IsMouseHoveringRect(bb.Max - ImVec2(20 + ImGui::CalcTextSize("Custom").x, 28), bb.Max - ImVec2(0, 12)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        it_circle->second = true;
        Beep(1, 250);
    }

    if (it_circle->second)
        ImGui::OpenPopup(identifier);
}
