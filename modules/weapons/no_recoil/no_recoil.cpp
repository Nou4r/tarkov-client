#include "no_recoil.hpp"

#include "../../../engine/engine.hpp"

#include "../../../renderer/menu/config/config.hpp"

int modules::weapon::math::generate_random_number(const int minimum_number, const int maximum_number)
{
	std::random_device device; std::mt19937 range(device());

	std::uniform_int_distribution< std::mt19937::result_type > distance(minimum_number, maximum_number);

	return distance(range);
}

void modules::weapon::no_recoil::callback()
{
	std::unique_lock<std::mutex> initiate_thread_locker(engine::values::global_locker);

	const auto camera_copy = engine::values::camera;

	const auto local_game_world_copy = engine::values::local_game_world;

	if (!camera_copy.instance || !local_game_world_copy.instance)
		return;

	const auto local_player_copy = local_game_world_copy.get_local_player();

	initiate_thread_locker.unlock();

	if (std::get< bool >(config::config_map[features::enable_no_recoil]))
	{
		if (!local_player_copy.instance)
			return;

		const auto local_player_shot_effector = local_player_copy.get_procedural_weapon_animation().get_shot_effector();

		if (!local_player_shot_effector.instance)
			return;

		engine::structures::vector2_t recoil_xy{ std::get< int >(config::config_map[features::recoil_x]), std::get< int >(config::config_map[features::recoil_y]) };
		engine::structures::vector2_t recoil_depth_xy{ std::get< int >(config::config_map[features::recoil_depth_x]), std::get< int >(config::config_map[features::recoil_depth_y]) };

		recoil_xy.x += modules::weapon::math::generate_random_number(std::get< int >(config::config_map[features::recoil_random_x]), std::get< int >(config::config_map[features::recoil_random_y]));
		recoil_xy.y += modules::weapon::math::generate_random_number(std::get< int >(config::config_map[features::recoil_random_x]), std::get< int >(config::config_map[features::recoil_random_y]));

		recoil_depth_xy.x += modules::weapon::math::generate_random_number(std::get< int >(config::config_map[features::recoil_random_x]), std::get< int >(config::config_map[features::recoil_random_y]));
		recoil_depth_xy.y += modules::weapon::math::generate_random_number(std::get< int >(config::config_map[features::recoil_random_x]), std::get< int >(config::config_map[features::recoil_random_y]));

		local_player_shot_effector.set_recoil_strength_xy(recoil_xy);
		local_player_shot_effector.set_recoil_strength_z(recoil_depth_xy);
	}
}