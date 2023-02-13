#include "../../../renderer/renderer.hpp"

#include "../../libs/imgui_tricks.hpp"

static const char* PatchFormatStringFloatToInt(const char* fmt)
{
    if (fmt[0] == '%' && fmt[1] == '.' && fmt[2] == '0' && fmt[3] == 'f' && fmt[4] == 0) // Fast legacy path for "%.0f" which is expected to be the most common case.
        return "%d";
    const char* fmt_start = ImParseFormatFindStart(fmt);    // Find % (if any, and ignore %%)
    const char* fmt_end = ImParseFormatFindEnd(fmt_start);  // Find end of format specifier, which itself is an exercise of confidence/recklessness (because snprintf is dependent on libc or user).
    if (fmt_end > fmt_start && fmt_end[-1] == 'f')
    {
        if (fmt_start == fmt && fmt_end[0] == 0)
            return "%d";
        ImGuiContext& g = *GImGui;
        ImFormatString(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), "%.*s%%d%s", (int)(fmt_start - fmt), fmt, fmt_end); // Honor leading and trailing decorations, but lose alignment/precision.
        return g.TempBuffer;
    }
    return fmt;
}

bool SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, float power)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos + ImVec2(0, 0), window->DC.CursorPos + ImVec2(328, label_size.y + style.FramePadding.y * 2.0f));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f + 9));

    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id, &frame_bb))
        return false;

    if (format == NULL)
        format = ImGui::DataTypeGetInfo(data_type)->PrintFmt;
    else if (data_type == ImGuiDataType_S32 && strcmp(format, "%d") != 0)
        format = PatchFormatStringFloatToInt(format);

    const bool hovered = ImGui::ItemHoverable(frame_bb, id);
    bool temp_input_is_active = ImGui::TempInputIsActive(id);
    bool temp_input_start = false;
    if (!temp_input_is_active)
    {
        const bool focus_requested = hovered && g.IO.MouseClicked[0];
        const bool clicked = (hovered && g.IO.MouseClicked[0]);
        if (focus_requested || clicked || g.NavActivateId == id || g.NavInputId == id)
        {
            ImGui::SetActiveID(id, window);
            ImGui::SetFocusID(id, window);
            ImGui::FocusWindow(window);
            g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
            if (focus_requested || (clicked && g.IO.KeyCtrl) || g.NavInputId == id)
            {
                temp_input_start = true;
            }
        }
    }

    ImGui::RenderFrame(frame_bb.Min + ImVec2(0, 5), frame_bb.Max - ImVec2(0, 7), ImColor(35, 42, 51), true, 6);

    ImRect grab_bb;
    const bool value_changed = ImGui::SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, ImGuiSliderFlags_None, &grab_bb);
    if (value_changed)
        ImGui::MarkItemEdited(id);

    if (grab_bb.Max.x > grab_bb.Min.x)
        window->DrawList->AddRectFilled(frame_bb.Min + ImVec2(0, 5), grab_bb.Max - ImVec2(0, 5), ImColor(138, 194, 227), 6);

    window->DrawList->AddCircleFilled(grab_bb.Max - ImVec2(5, 9), 11, ImColor(19, 19, 19), 32);
    window->DrawList->AddCircleFilled(grab_bb.Max - ImVec2(5, 9), 8, ImColor(211, 211, 211, 211), 32);

    char value_buf[64];
    const char* value_buf_end = value_buf + ImGui::DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

    if (label_size.x > 0.0f)
        ImGui::RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    return value_changed;
}

bool renderer::imgui::elements::functions::slider_float(const char* label, float* v, float v_min, float v_max, const char* format, float power)
{
    return SliderScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, format, power);
}

bool renderer::imgui::elements::functions::slider_int(const char* label, int* v, int v_min, int v_max, const char* format)
{
    return SliderScalar(label, ImGuiDataType_S32, v, &v_min, &v_max, format, 1);
}

bool SliderScalarStretched(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, float power)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos + ImVec2(10, 0), window->DC.CursorPos + ImVec2(268, label_size.y + style.FramePadding.y * 2.0f));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id, &frame_bb))
        return false;

    if (format == NULL)
        format = ImGui::DataTypeGetInfo(data_type)->PrintFmt;
    else if (data_type == ImGuiDataType_S32 && strcmp(format, "%d") != 0)
        format = PatchFormatStringFloatToInt(format);

    const bool hovered = ImGui::ItemHoverable(frame_bb, id);
    bool temp_input_is_active = ImGui::TempInputIsActive(id);
    bool temp_input_start = false;
    if (!temp_input_is_active)
    {
        const bool focus_requested = hovered && g.IO.MouseClicked[0];
        const bool clicked = (hovered && g.IO.MouseClicked[0]);
        if (focus_requested || clicked || g.NavActivateId == id || g.NavInputId == id)
        {
            ImGui::SetActiveID(id, window);
            ImGui::SetFocusID(id, window);
            ImGui::FocusWindow(window);
            g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
            if (focus_requested || (clicked && g.IO.KeyCtrl) || g.NavInputId == id)
            {
                temp_input_start = true;
            }
        }
    }

    ImGui::RenderFrame(frame_bb.Min + ImVec2(0, 5), frame_bb.Max - ImVec2(0, 6), ImColor(35, 42, 51), true, 6);

    ImRect grab_bb;
    const bool value_changed = ImGui::SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, ImGuiSliderFlags_None, &grab_bb);
    if (value_changed)
        ImGui::MarkItemEdited(id);

    if (grab_bb.Max.x > grab_bb.Min.x)
        window->DrawList->AddRectFilled(frame_bb.Min + ImVec2(0, 5), grab_bb.Max - ImVec2(0, 5), ImColor(138, 194, 227), 6);

    window->DrawList->AddCircleFilled(grab_bb.Max - ImVec2(5, 9), 12, ImColor(28, 34, 42), 32);
    window->DrawList->AddCircleFilled(grab_bb.Max - ImVec2(5, 9), 7, ImColor(211, 211, 211, 211), 32);

    char value_buf[64];
    const char* value_buf_end = value_buf + ImGui::DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

    ImGui::RenderText(ImVec2(frame_bb.Max.x + 40 - ImGui::CalcTextSize(value_buf).x, frame_bb.Min.y + 1), value_buf);

    return value_changed;
}

bool renderer::imgui::elements::functions::slider_float_stretched(const char* label, float* v, float v_min, float v_max, const char* format, float power)
{
    return SliderScalarStretched(label, ImGuiDataType_Float, v, &v_min, &v_max, format, power);
}

bool renderer::imgui::elements::functions::slider_int_stretched(const char* label, int* v, int v_min, int v_max, const char* format)
{
    return SliderScalarStretched(label, ImGuiDataType_S32, v, &v_min, &v_max, format, 1);
}
