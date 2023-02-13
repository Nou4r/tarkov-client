#include "../../../renderer/renderer.hpp"

#include "../../libs/imgui_tricks.hpp"

const char* const KeyNames[] = {
    "Unknown",
    "VK_LBUTTON",
    "VK_RBUTTON",
    "VK_CANCEL",
    "VK_MBUTTON",
    "VK_XBUTTON1",
    "VK_XBUTTON2",
    "Unknown",
    "VK_BACK",
    "VK_TAB",
    "Unknown",
    "Unknown",
    "VK_CLEAR",
    "VK_RETURN",
    "Unknown",
    "Unknown",
    "VK_SHIFT",
    "VK_CONTROL",
    "VK_MENU",
    "VK_PAUSE",
    "VK_CAPITAL",
    "VK_KANA",
    "Unknown",
    "VK_JUNJA",
    "VK_FINAL",
    "VK_KANJI",
    "Unknown",
    "VK_ESCAPE",
    "VK_CONVERT",
    "VK_NONCONVERT",
    "VK_ACCEPT",
    "VK_MODECHANGE",
    "VK_SPACE",
    "VK_PRIOR",
    "VK_NEXT",
    "VK_END",
    "VK_HOME",
    "VK_LEFT",
    "VK_UP",
    "VK_RIGHT",
    "VK_DOWN",
    "VK_SELECT",
    "VK_PRINT",
    "VK_EXECUTE",
    "VK_SNAPSHOT",
    "VK_INSERT",
    "VK_DELETE",
    "VK_HELP",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "VK_LWIN",
    "VK_RWIN",
    "VK_APPS",
    "Unknown",
    "VK_SLEEP",
    "VK_NUMPAD0",
    "VK_NUMPAD1",
    "VK_NUMPAD2",
    "VK_NUMPAD3",
    "VK_NUMPAD4",
    "VK_NUMPAD5",
    "VK_NUMPAD6",
    "VK_NUMPAD7",
    "VK_NUMPAD8",
    "VK_NUMPAD9",
    "VK_MULTIPLY",
    "VK_ADD",
    "VK_SEPARATOR",
    "VK_SUBTRACT",
    "VK_DECIMAL",
    "VK_DIVIDE",
    "VK_F1",
    "VK_F2",
    "VK_F3",
    "VK_F4",
    "VK_F5",
    "VK_F6",
    "VK_F7",
    "VK_F8",
    "VK_F9",
    "VK_F10",
    "VK_F11",
    "VK_F12",
    "VK_F13",
    "VK_F14",
    "VK_F15",
    "VK_F16",
    "VK_F17",
    "VK_F18",
    "VK_F19",
    "VK_F20",
    "VK_F21",
    "VK_F22",
    "VK_F23",
    "VK_F24",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "VK_NUMLOCK",
    "VK_SCROLL",
    "VK_OEM_NEC_EQUAL",
    "VK_OEM_FJ_MASSHOU",
    "VK_OEM_FJ_TOUROKU",
    "VK_OEM_FJ_LOYA",
    "VK_OEM_FJ_ROYA",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "VK_LSHIFT",
    "VK_RSHIFT",
    "VK_LCONTROL",
    "VK_RCONTROL",
    "VK_LMENU",
    "VK_RMENU"
};

bool renderer::imgui::elements::functions::hotkey(const char* label, int* k)
{
    using namespace ImGui;

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImGuiIO& io = g.IO;
    const ImGuiStyle& style = g.Style;

    const ImGuiID id = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImGui::CalcItemSize({ 328, 44 + 5 }, 328, 44);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    const bool hovered = ImGui::ItemHoverable(bb, id);

    if (hovered) {
        ImGui::SetHoveredID(id);
        g.MouseCursor = ImGuiMouseCursor_TextInput;
    }

    const bool user_clicked = hovered && io.MouseClicked[0];

    if (user_clicked) {
        if (g.ActiveId != id) {
            // Start edition
            memset(io.MouseDown, 0, sizeof(io.MouseDown));
            memset(io.KeysDown, 0, sizeof(io.KeysDown));
            *k = 0;
        }
        ImGui::SetActiveID(id, window);
        ImGui::FocusWindow(window);
    }
    else if (io.MouseClicked[0]) {
        // Release focus when we click outside
        if (g.ActiveId == id)
            ImGui::ClearActiveID();
    }

    bool value_changed = false;
    int key = *k;

    if (g.ActiveId == id) {
        for (auto i = 0; i <= 6; i++) {
            if (io.MouseDown[i] || GetAsyncKeyState(VK_XBUTTON1) || GetAsyncKeyState(VK_XBUTTON2)) {
                switch (i) {
                case 0:
                    key = VK_LBUTTON;
                    break;
                case 1:
                    key = VK_RBUTTON;
                    break;
                case 2:
                    key = VK_MBUTTON;
                    break;
                }
                if (GetAsyncKeyState(VK_XBUTTON2))
                    key = VK_XBUTTON2;
                if (GetAsyncKeyState(VK_XBUTTON1))
                    key = VK_XBUTTON1;

                value_changed = true;
                ImGui::ClearActiveID();
            }
        }
        if (!value_changed) {
            for (auto i = VK_BACK; i <= VK_RMENU; i++) {
                if (GetAsyncKeyState(i)) {
                    key = i;
                    value_changed = true;
                    ImGui::ClearActiveID();
                }
            }
        }

        if (ImGui::IsKeyPressedMap(ImGuiKey_Escape)) {
            *k = 0;
            ImGui::ClearActiveID();
        }
        else {
            *k = key;
        }
    }

    char buf_display[64] = "NONE";

    ImGui::RenderFrame(bb.Min + ImVec2(0, 0), bb.Max - ImVec2(0, 10), ImColor(33, 33, 33), false, 4);

    if (*k != 0 && g.ActiveId != id) {
        strcpy_s(buf_display, KeyNames[*k]);
    }
    else if (g.ActiveId == id) {
        strcpy_s(buf_display, "Press a key");
    }

    float Active = ImTricks::Animations::FastFloatLerp(id, hovered, 0.f, 1.f, 0.05f);

    if (g.ActiveId == id)
        window->DrawList->AddText(fonts::px18, 18, bb.Min + ImVec2(328 / 2 - (ImGui::CalcTextSize(buf_display).x / 2) * 1.2f, 10), ImColor(211, 211, 211, 160 + int(Active * 91.f)), buf_display);
    else
        window->DrawList->AddText(fonts::px18, 18, bb.Min + ImVec2(50, 10), ImColor(211, 211, 211, 160 + int(Active * 91.f)), buf_display),
        window->DrawList->AddText(NULL, 13, bb.Min + ImVec2(18, 15), ImColor(211, 211, 211, 160 + int(Active * 91.f)), (const char*)ICON_FA_LAYER_GROUP);

    return value_changed;
}

bool renderer::imgui::elements::functions::hotkey_stretched(const char* label, int* k)
{
    using namespace ImGui;

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImGuiIO& io = g.IO;
    const ImGuiStyle& style = g.Style;

    const ImGuiID id = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImGui::CalcItemSize({ 318, 28 + 5 }, 318, 28 + 5);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    const bool hovered = ImGui::ItemHoverable(bb, id);

    if (hovered) {
        ImGui::SetHoveredID(id);
        g.MouseCursor = ImGuiMouseCursor_TextInput;
    }

    const bool user_clicked = hovered && io.MouseClicked[0];

    if (user_clicked) {
        if (g.ActiveId != id) {
            // Start edition
            memset(io.MouseDown, 0, sizeof(io.MouseDown));
            memset(io.KeysDown, 0, sizeof(io.KeysDown));
            *k = 0;
        }
        ImGui::SetActiveID(id, window);
        ImGui::FocusWindow(window);
    }
    else if (io.MouseClicked[0]) {
        // Release focus when we click outside
        if (g.ActiveId == id)
            ImGui::ClearActiveID();
    }

    bool value_changed = false;
    int key = *k;

    if (g.ActiveId == id) {
        for (auto i = 0; i <= 6; i++) {
            if (io.MouseDown[i] || GetAsyncKeyState(VK_XBUTTON1) || GetAsyncKeyState(VK_XBUTTON2)) {
                switch (i) {
                case 0:
                    key = VK_LBUTTON;
                    break;
                case 1:
                    key = VK_RBUTTON;
                    break;
                case 2:
                    key = VK_MBUTTON;
                    break;
                }
                if (GetAsyncKeyState(VK_XBUTTON2))
                    key = VK_XBUTTON2;
                if (GetAsyncKeyState(VK_XBUTTON1))
                    key = VK_XBUTTON1;

                value_changed = true;
                ImGui::ClearActiveID();
            }
        }
        if (!value_changed) {
            for (auto i = VK_BACK; i <= VK_RMENU; i++) {
                if (GetAsyncKeyState(i)) {
                    key = i;
                    value_changed = true;
                    ImGui::ClearActiveID();
                }
            }
        }

        if (ImGui::IsKeyPressedMap(ImGuiKey_Escape)) {
            *k = 0;
            ImGui::ClearActiveID();
        }
        else {
            *k = key;
        }
    }

    char buf_display[64] = "NONE";

    ImGui::RenderFrame(bb.Min + ImVec2(130, 0), bb.Max - ImVec2(10, 5), ImColor(35, 42, 51), false, 4);

    if (*k != 0 && g.ActiveId != id) {
        strcpy_s(buf_display, KeyNames[*k]);
    }
    else if (g.ActiveId == id) {
        strcpy_s(buf_display, "<Press a key>");
    }

    float Active = ImTricks::Animations::FastFloatLerp(id, hovered, 0.f, 1.f, 0.05f);

    window->DrawList->AddText(NULL, 7, bb.Min + ImVec2(18 + 120, 13), ImColor(211, 211, 211, 160 + int(Active * 91.f)), (const char*)ICON_FA_KEYBOARD);
    window->DrawList->AddText(bb.Min + ImVec2(40 + 120, 7), ImColor(211, 211, 211, 160 + int(Active * 91.f)), buf_display);
    window->DrawList->AddText(bb.Min + ImVec2(10, 7), ImColor(211, 211, 211, 211), label);

    return value_changed;
}
