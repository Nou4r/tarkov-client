#pragma once

#include <list>

#include "../../../engine/engine.hpp"

namespace modules::visuals::grenades
{
	void callback(engine::types::camera_t camera_copy, engine::types::local_game_world_t local_game_world_copy, const std::vector< grenade_structure_t >& grenades_copy, engine::types::player_t local_player_copy, engine::types::player_t closest_player, engine::structures::vector3_t lp_pos);
}