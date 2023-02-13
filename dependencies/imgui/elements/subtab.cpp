#include "../../../renderer/renderer.hpp"
#include "../../libs/imgui_tricks.hpp"

#include <map>

struct TabStruct {
	float size;
	float hovered;
};

bool renderer::imgui::elements::functions::subtab_static(const char* label, const char* icon, int& tab, int index)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	const ImGuiID id = window->GetID(label);
	
	std::map<ImGuiID, TabStruct> circle_anim;

	auto it_circle = circle_anim.find(id);
	if (it_circle == circle_anim.end())
	{
		circle_anim.insert({ id, {50, 0} });
		it_circle = circle_anim.find(id);
	}

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size = ImGui::CalcItemSize({ 74, 66 }, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	ImRect bb(pos, pos + size);
	ImGui::InvisibleButton(label, size);

	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(7);

	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		tab = index;

	it_circle->second.hovered = ImTricks::Animations::FastFloatLerp(window->GetID(std::string(label).append("hover").c_str()), index == tab, 0.f, 1.f, 0.0625f);

	ImGui::PushClipRect(bb.Min, bb.Max, false);

	window->DrawList->AddRectFilled(bb.Min, bb.Max, ImColor(32, 39, 49, int(it_circle->second.hovered * 255)), 7);

	window->DrawList->AddText(NULL, 9, bb.Min + ImVec2(37 - 13 / 2, 25), ImTricks::Animations::FastColorLerp(
		ImColor(210, 210, 210, 160),
		ImColor(211, 243, 107),
		it_circle->second.hovered), icon);

	window->DrawList->AddText(NULL, 12, bb.Min + ImVec2(39 - (ImGui::CalcTextSize(label).x * 0.85f) / 2, 45), ImTricks::Animations::FastColorLerp(
		ImColor(210, 210, 210, 160),
		ImColor(211, 243, 107),
		it_circle->second.hovered),
		label);

	ImGui::PopClipRect();
}