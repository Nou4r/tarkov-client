#include "silent.hpp"

#include "../../../engine/engine.hpp"

#include "../../../renderer/renderer.hpp"

#include "../../../renderer/menu/config/config.hpp"

#include "../../../dependencies/xorstring/xorstring.hpp"
#include "../../../settings/settings.hpp"

void modules::aimbot::silent::callback()
{
	//while (true)
	//{
	//	if (!settings::values::premium || !std::get< bool >(config::config_map[features::aimbot_enable"]))
	//		continue;

	//	if (!GetAsyncKeyState(std::get< int >(config::config_map[features::aimbot_hotkey"])))
	//	{
	//		std::unique_lock<std::mutex> update_players_locker(engine::values::aim_player_vector_locker);

	//		engine::values::closest_player = {};

	//		update_players_locker.unlock();

	//		continue;
	//	}

	//	std::unique_lock<std::mutex> initiate_thread_locker(engine::values::global_locker);

	//	const auto camera_copy = engine::values::camera;

	//	const auto local_game_world_copy = engine::values::local_game_world;

	//	initiate_thread_locker.unlock();

	//	if (!camera_copy.instance || !local_game_world_copy.instance)
	//		continue;

	//	std::unique_lock<std::mutex> update_players_locker(engine::values::aim_player_vector_locker);

	//	const auto local_player_copy = local_game_world_copy.get_local_player();

	//	const auto closest_player_copy = engine::values::closest_player;

	//	const auto velocity = engine::values::velocity;

	//	update_players_locker.unlock();

	//	if (closest_player_copy.instance)
	//	{
	//		const auto movement_context = driver.read(local_player_copy.instance + 0x40);

	//		auto from = driver.read< engine::types::transform_t >(local_player_copy.instance, { 0x558, 0x140, 0x10 }).get_position();

	//		auto head_world_position = closest_player_copy.get_bone_transform(engine::structures::bones::head).get_position();

	//		const auto head_position = camera_copy.world_to_screen(head_world_position);

	//		if (!head_position.has_value())
	//			continue;

	//		const auto head_screen_position = head_position.value();

	//		const auto w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);

	//		const auto screen_distance = std::sqrtf(std::powf(head_screen_position.x - w / 2, 2) + std::powf(head_screen_position.y - h / 2, 2));

	//		if (std::get< bool >(config::config_map[features::aimbot_show_fov"]))
	//		{
	//			if (std::get<int>(config::config_map[features::aimbot_fov"]) * 10.f < screen_distance)
	//				continue;
	//		}

	//		engine::structures::vector3_t delta = from - head_world_position; engine::structures::rotator_t ret;

	//		float length = sqrt(delta.squared());

	//		ret.y = std::asinf(delta.y / length); ret.x = -std::atan2f(delta.x, -delta.z);

	//		auto res = ret * 57.29578f;

	//		driver.write< engine::structures::vector2_t >(movement_context + 0x22C, { res.x, res.y });

	//		const auto pwa = local_player_copy.get_procedural_weapon_animation();
	//		const auto v1 = driver.read(pwa.instance, { 0x80, 0xE0, 0x18 });
	//		const auto sz = driver.read< std::uint32_t >(v1 + engine::offsets::generic_list::size);
	//		const auto ls = driver.read(v1 + engine::offsets::generic_list::elements) + engine::offsets::generic_list::first_element;

	//		if (!ls)
	//			continue;

	//		for (auto iterator = 0u; iterator < sz; ++iterator)
	//		{
	//			const auto item = driver.read(ls + (iterator * sizeof(void*)));

	//			if (!item)
	//				continue;

	//			const auto player = driver.read(item + 0x58);

	//			const auto parent_player = driver.read(driver.read(item + 0x68));

	//			if (player != local_player_copy.instance && parent_player != local_player_copy.instance)
	//				continue;

	//			if (driver.read<bool>(item + 0x160)) //isprocessed
	//				continue;

	//			const auto trajectory_info_list = driver.read(item + 0x30);

	//			if (!trajectory_info_list)
	//				continue;

	//			const auto traj_size = driver.read<std::uint32_t>(trajectory_info_list + 0x18);

	//			struct traj_t
	//			{
	//				float time;
	//				engine::structures::vector3_t position, velocity;
	//			};

	//			const auto trajectories = std::make_unique<traj_t[]>(traj_size);

	//			driver.read_arr(trajectory_info_list + 0x20, trajectories.get(), traj_size);

	//			trajectories[0].velocity = velocity * (driver.read<engine::structures::vector3_t>(item + 0xF8)).magnitude();

	//			for (auto i = 1u; i < traj_size; ++i)
	//			{
	//				const auto& last_trajectory = trajectories[i - 1];
	//				auto& new_trajectory = trajectories[i];

	//				new_trajectory.position = last_trajectory.position + last_trajectory.velocity * 0.01f;
	//				new_trajectory.velocity = last_trajectory.velocity * 0.5f;
	//			}

	//			driver.write_arr(trajectory_info_list + 0x20, trajectories.get(), traj_size);
	//		}
	//	}
	//	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	//}
}

void modules::aimbot::silent::visual::draw_fov()
{
	if (std::get< bool >(config::config_map[features::aimbot_show_fov]) && std::get< int >(config::config_map[features::aimbot_fov]) > 0)
	{
		const auto w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);

		const auto draw_list = ImGui::GetBackgroundDrawList();

		const auto col = std::get< engine::structures::vector4_t >(config::config_map[features::fov_color]);

		draw_list->AddCircle({ w / 2.f, h / 2.f }, std::get< int >(config::config_map[features::aimbot_fov]) * 10, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, std::get< int >(config::config_map[features::aimbot_fov_segments]), 2.f);
	}
}