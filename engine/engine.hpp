#pragma once

#include <mutex>

#include "types/types.hpp"
#include "offsets/offsets.hpp"

namespace engine::values
{
	extern std::mutex global_locker;
	extern std::mutex player_vector_locker;

	extern std::mutex loot_vector_locker;
	extern std::mutex grenade_vector_locker;

	extern std::uintptr_t unity_player;
	extern std::uintptr_t mono_dll;

	extern types::game_object_manager_t game_object_manager;
	extern types::camera_t camera;
	extern types::local_game_world_t local_game_world;

	extern std::vector< types::player_t > players;
	
	extern std::vector< loot_structure_t > loot;
	extern std::vector< grenade_structure_t > grenades;

	extern std::mutex aim_player_vector_locker;

	extern std::mutex closest_player_locker;

	extern std::vector< types::player_t > aim_players;

	extern types::player_t closest_player;

	extern engine::structures::vector3_t velocity;

	extern std::uintptr_t hard_settings;
	extern std::uintptr_t inertia_settings;
	extern std::uintptr_t init_shot;
}

namespace engine::tools
{
	std::pair< std::int32_t, std::int32_t > get_screen_size();
}

namespace engine::functions
{
	void initiate( );

	void update_closest_player();
	void update_players_vector( );
	
	void update_loot_vector( );
	void update_grenade_vector( );

	void update_aim_players_vector();
}