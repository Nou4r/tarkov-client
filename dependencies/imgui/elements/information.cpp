#include "../../../renderer/renderer.hpp"

#include "../../libs/imgui_tricks.hpp"

int DataTypeFormatString(char* buf, int buf_size, ImGuiDataType data_type, const void* p_data, const char* format)
{
    if (data_type == ImGuiDataType_S32 || data_type == ImGuiDataType_U32)
        return ImFormatString(buf, buf_size, format, *(const ImU32*)p_data);
    if (data_type == ImGuiDataType_S64 || data_type == ImGuiDataType_U64)
        return ImFormatString(buf, buf_size, format, *(const ImU64*)p_data);
    if (data_type == ImGuiDataType_Float)
        return ImFormatString(buf, buf_size, format, *(const float*)p_data);
    if (data_type == ImGuiDataType_Double)
        return ImFormatString(buf, buf_size, format, *(const double*)p_data);
    if (data_type == ImGuiDataType_S8)
        return ImFormatString(buf, buf_size, format, *(const ImS8*)p_data);
    if (data_type == ImGuiDataType_U8)
        return ImFormatString(buf, buf_size, format, *(const ImU8*)p_data);
    if (data_type == ImGuiDataType_S16)
        return ImFormatString(buf, buf_size, format, *(const ImS16*)p_data);
    if (data_type == ImGuiDataType_U16)
        return ImFormatString(buf, buf_size, format, *(const ImU16*)p_data);
    IM_ASSERT(0);
    return 0;
}

void renderer::imgui::elements::functions::information(const char* label, const char* desc, float p_data, const char* format)
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
    ImVec2 size = ImGui::CalcItemSize({ 318, 57 }, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

    char value_buf[64];
    const char* value_buf_end = value_buf + ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), format, p_data);

    window->DrawList->AddText(fonts::px18, 18, bb.Min + ImVec2(0, 12), ImColor(228, 228, 230), label);
    window->DrawList->AddText(bb.Min + ImVec2(0, 31), ImColor(156, 167, 190, 125), desc);

    window->DrawList->AddText(NULL, 15, bb.Min + ImVec2(328 - ImGui::CalcTextSize(value_buf).x - 25, 57 / 2 - ImGui::CalcTextSize((const char*)ICON_FA_RADAR).y / 2), ImColor(211, 211, 211, 211), (const char*)ICON_FA_RADAR);
    window->DrawList->AddText(fonts::px18, 18, bb.Min + ImVec2(328 - ImGui::CalcTextSize(value_buf).x, 23 - ImGui::CalcTextSize(value_buf).y / 2), ImColor(211, 211, 211, 211), value_buf);
}

void renderer::imgui::elements::functions::information(const char* label, const char* desc)
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
    ImVec2 size = ImGui::CalcItemSize({ 318, 57 }, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

    window->DrawList->AddText(fonts::px18, 18, bb.Min + ImVec2(0, 12), ImColor(228, 228, 230), label);
    window->DrawList->AddText(bb.Min + ImVec2(0, 31), ImColor(156, 167, 190, 125), desc);
}

void renderer::imgui::elements::functions::information(const char* label, const char* desc, int p_data, const char* format)
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
    ImVec2 size = ImGui::CalcItemSize({ 318, 52 }, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

    char value_buf[64];
    const char* value_buf_end = value_buf + ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), format, p_data);

    window->DrawList->AddText(fonts::px18, 18, bb.Min + ImVec2(0, 12), ImColor(228, 228, 230), label);
    window->DrawList->AddText(bb.Min + ImVec2(0, 31), ImColor(156, 167, 190, 125), desc);

    window->DrawList->AddText(NULL, 15, bb.Min + ImVec2(328 - ImGui::CalcTextSize(value_buf).x - 25, 57 / 2 - ImGui::CalcTextSize((const char*)ICON_FA_RADAR).y / 2), ImColor(211, 211, 211, 211), (const char*)ICON_FA_RADAR);
    window->DrawList->AddText(fonts::px18, 18, bb.Min + ImVec2(328 - ImGui::CalcTextSize(value_buf).x, 23 - ImGui::CalcTextSize(value_buf).y / 2), ImColor(211, 211, 211, 211), value_buf);
}

void renderer::imgui::elements::functions::information(const char* label, const char* desc, const char* p_data, const char* icon)
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
    ImVec2 size = ImGui::CalcItemSize({ 328, 52 }, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

    window->DrawList->AddText(fonts::px18, 18, bb.Min + ImVec2(0, 12), ImColor(228, 228, 230), label);
    window->DrawList->AddText(bb.Min + ImVec2(0, 31), ImColor(156, 167, 190, 125), desc);

    window->DrawList->AddText(NULL, 15, bb.Min + ImVec2(328 - ImGui::CalcTextSize(p_data).x - 25, 57 / 2 - ImGui::CalcTextSize(icon).y / 2), ImColor(211, 211, 211, 211), icon);
    window->DrawList->AddText(fonts::px18, 18, bb.Min + ImVec2(328 - ImGui::CalcTextSize(p_data).x, 23 - ImGui::CalcTextSize(p_data).y / 2), ImColor(211, 211, 211, 211), p_data);
}

