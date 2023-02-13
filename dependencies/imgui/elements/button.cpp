#include "../../../renderer/renderer.hpp"

#include "../../libs/imgui_tricks.hpp"

bool renderer::imgui::elements::functions::button(const char* label, ImVec2 size_arg, renderer::imgui::elements::button_flags flag)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	int flags = 0;
	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

	if (hovered)
		ImGui::SetMouseCursor(7);

	float Hovered = ImTricks::Animations::FastFloatLerp(id, hovered, 0.f, 1.f, 0.09f);

	if (flag == BUTTON_GRAY) {
		window->DrawList->AddRectFilled(bb.Min, bb.Max, ImTricks::Animations::FastColorLerp(ImColor(32, 39, 49, 255), ImColor(73, 177, 116), Hovered), 5);
	}
	else if (flag == BUTTON_GREEN) {
		window->DrawList->AddRectFilled(bb.Min, bb.Max, ImTricks::Animations::FastColorLerp(ImColor(73, 177, 116, 255), ImColor(49, 142, 87), Hovered), 5);
	}
	else if (flag == BUTTON_BLUE) {
		window->DrawList->AddRectFilled(bb.Min, bb.Max, ImTricks::Animations::FastColorLerp(ImColor(138, 194, 227, 255), ImColor(82, 131, 161), Hovered), 5);
	}
	else if (flag == BUTTON_RED) {
		window->DrawList->AddRectFilled(bb.Min, bb.Max, ImTricks::Animations::FastColorLerp(ImColor(255, 82, 82, 255), ImColor(186, 60, 60), Hovered), 5);
	}
	else if (flag == BUTTON_PURPLE) {
		window->DrawList->AddRectFilled(bb.Min, bb.Max, ImTricks::Animations::FastColorLerp(ImColor(127, 128, 246, 255), ImColor(93, 94, 189), Hovered), 5);
	}

	window->DrawList->AddText(bb.Min + ImVec2(size_arg.x / 2 - ImGui::CalcTextSize(label).x / 2, size_arg.y / 2 - ImGui::CalcTextSize(label).y / 2), ImColor(230, 230, 230), label);

	return pressed;
}
