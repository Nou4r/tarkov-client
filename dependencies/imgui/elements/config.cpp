#include "../../../renderer/renderer.hpp"

#include "../../libs/imgui_tricks.hpp"

#include <map>

struct anim {
    float size;
    float hover;
};

bool renderer::imgui::elements::functions::config_button(const char* label, const char* data, const char* author)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImGuiID id = window->GetID(label);

    std::map<ImGuiID, anim> circle_anim;

    auto it_circle = circle_anim.find(id);
    if (it_circle == circle_anim.end())
    {
        circle_anim.insert({ id, {61.f, 0.f} });
        it_circle = circle_anim.find(id);
    }

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImGui::CalcItemSize({ 328.f, it_circle->second.size }, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    ImRect bb(pos, pos + size);
    ImGui::InvisibleButton(label, size);


    if (ImGui::IsItemHovered())
    {
        if (it_circle->second.hover <= 1.f)
            it_circle->second.hover += 0.05f;
    }
    else if (!ImGui::IsItemHovered())
    {
        if (it_circle->second.hover >= 0.f)
            it_circle->second.hover -= 0.05f;
    }

    it_circle->second.size = ImTricks::Animations::FastFloatLerp(id, ImGui::IsItemHovered(), 61.f, 115.f, 5.f);

    window->DrawList->AddRect(bb.Min, bb.Max, ImColor(255, 255, 255, 20), 5);
    window->DrawList->AddText(bb.Min + ImVec2(15, 13), ImColor(230, 230, 230), label);
    window->DrawList->AddText(bb.Min + ImVec2(15, 34), ImColor(160, 160, 160), "data: ");
    window->DrawList->AddText(bb.Min + ImVec2(15 + ImGui::CalcTextSize("data: ").x, 34), ImColor(230, 230, 230), data);


    window->DrawList->AddText(bb.Min + ImVec2(125, 34), ImColor(160, 160, 160), "author: ");
    window->DrawList->AddText(bb.Min + ImVec2(125 + ImGui::CalcTextSize("author: ").x, 34), ImColor(127, 128, 246), author);

    if (it_circle->second.size > 65.f)
    {
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(7);

        window->DrawList->AddRectFilled(bb.Min + ImVec2(15, 60), bb.Min + ImVec2(120, 100), ImColor(127, 128, 246, int(it_circle->second.hover * 255)), 5);
        window->DrawList->AddText(bb.Min + ImVec2(15 + 55 - ImGui::CalcTextSize("LOAD").x / 2, 58 + (120 - 75) / 2 - ImGui::CalcTextSize("LOAD").y / 2), ImColor(245, 245, 245, int(it_circle->second.hover * 255)), "LOAD");

        window->DrawList->AddRectFilled(bb.Min + ImVec2(130, 60), bb.Min + ImVec2(170, 100), ImColor(33, 33, 33), 5);
        window->DrawList->AddText(bb.Min + ImVec2(130 + 20 - ImGui::CalcTextSize((const char*)ICON_FA_TIMES).x / 2, 60 + (120 - 75) / 2 - ImGui::CalcTextSize((const char*)ICON_FA_TIMES).y / 2), ImColor(203, 44, 55), (const char*)ICON_FA_TIMES);


        return ImGui::IsItemClicked();
    }
}
