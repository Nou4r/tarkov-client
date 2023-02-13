#include "aimbot.hpp"

#include "../../engine/engine.hpp"

#include "../../renderer/renderer.hpp"

#include "../../renderer/menu/config/config.hpp"

#include "../../dependencies/xorstring/xorstring.hpp"
#include "../../settings/settings.hpp"

auto player_bone_map = std::map< int, engine::structures::bones >
{
	{ 0, engine::structures::bones::head },
	{ 1, engine::structures::bones::r_collarbone },
	{ 2, engine::structures::bones::pelvis },
	{ 3, engine::structures::bones::l_thigh_2 },
	{ 4, engine::structures::bones::r_thigh_2 }
};

void modules::aimbot::visual::draw_fov()
{
	if (std::get< bool >(config::config_map[features::aimbot_show_fov]) && std::get< int >(config::config_map[features::aimbot_fov]) > 0)
	{
		const auto w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);

		const auto draw_list = ImGui::GetBackgroundDrawList();

		const auto col = std::get< engine::structures::vector4_t >(config::config_map[features::fov_color]);

		draw_list->AddCircle({ w / 2.f, h / 2.f }, std::get< int >(config::config_map[features::aimbot_fov]) * 10, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, std::get< int >(config::config_map[features::aimbot_fov_segments]), 2.f);
	}
}

std::pair< engine::structures::vector2_t, engine::types::player_t > modules::aimbot::components::get_filtered_targets()
{
	std::unique_lock<std::mutex> initiate_thread_locker(engine::values::global_locker);

	const auto camera_copy = engine::values::camera;
	const auto local_game_world_copy = engine::values::local_game_world;

	initiate_thread_locker.unlock();

	std::unique_lock<std::mutex> update_aimbot_entities_locker(engine::values::aim_player_vector_locker);

	const auto aimbot_entities_copy = engine::values::aim_players;

	const auto local_entity_copy = local_game_world_copy.get_local_player(); auto best_distance = 10000.f;

	engine::types::player_t filtered_target{ };
	engine::structures::vector2_t filtered_target_position{ };

	if (!camera_copy.instance || !local_game_world_copy.instance)
		return std::pair(filtered_target_position, filtered_target);

	update_aimbot_entities_locker.unlock();

	for (const auto& entity : aimbot_entities_copy)
	{
		if (!entity.instance || !local_entity_copy.instance || entity.instance == local_entity_copy.instance)
			continue;

		if ((entity.get_profile_controller().get_profile_information().player_type() != xs("Player")) 
			&& std::get< bool >(config::config_map[features::aimbot_ignore_ai]))
			continue;

		const auto selected_player_bone = entity.get_bone_transform(player_bone_map[std::get< int >(config::config_map[features::aimbot_bone])]).get_position();
		const auto root_world_position = local_entity_copy.get_bone_transform(engine::structures::bones::root).get_position();

		engine::structures::vector2_t selected_bone_screen_position;

		const auto bone_position = camera_copy.world_to_screen(selected_player_bone);

		if (!bone_position.has_value())
			continue;

		selected_bone_screen_position = bone_position.value();

		const auto distance = static_cast<std::uint32_t>(root_world_position.distance(selected_player_bone));

		if (distance > std::get< int >(config::config_map[features::aimbot_max_distance]))
			continue;

		const auto [w, h] = engine::tools::get_screen_size();

		auto x = selected_bone_screen_position.x - w / 2.f;
		auto y = selected_bone_screen_position.y - h / 2.f;

		auto crosshair_distance = std::sqrtf((x * x) + (y * y));

		if (crosshair_distance <= 10000.f && crosshair_distance <= best_distance)
		{
			if (crosshair_distance > std::get< int >(config::config_map[features::aimbot_fov]) * 10)
				continue;

			best_distance = crosshair_distance;

			filtered_target_position = selected_bone_screen_position;
			filtered_target = entity;
		}
	}

	return std::pair(filtered_target_position, filtered_target);

	update_aimbot_entities_locker.lock();
}

void move_to(engine::structures::vector2_t aim)
{
	const auto [width, height] = engine::tools::get_screen_size();

	INPUT i = { };
	i.type = INPUT_MOUSE; i.mi.dwFlags = MOUSEEVENTF_MOVE;

	i.mi.dx = (aim.x - width / 2) / std::get< int >(config::config_map[features::aimbot_smoothness]);

	i.mi.dy = (aim.y - height / 2) / std::get< int >(config::config_map[features::aimbot_smoothness]);

	SendInput(1, &i, sizeof(INPUT));
}

void modules::aimbot::callback()
{
	while (true)
	{
		if (!std::get< bool >(config::config_map[features::aimbot_enable]))
			continue;

		if (!engine::values::local_game_world.instance || !engine::values::camera.instance)
			continue;

		auto fltered_target = components::get_filtered_targets();

		if (!fltered_target.second.instance)
			continue;

		if (!GetAsyncKeyState(std::get< int >(config::config_map[features::aimbot_hotkey])))
			continue;
		else
			move_to(fltered_target.first); std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}