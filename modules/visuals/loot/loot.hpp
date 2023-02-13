#pragma once

#include <list>

#include "../../../engine/engine.hpp"

namespace modules::visuals::loot
{
	void callback(engine::types::camera_t camera_copy, engine::types::local_game_world_t local_game_world_copy, const std::vector< loot_structure_t >& items_copy, engine::types::player_t local_player_copy, engine::types::player_t closest_player, engine::structures::vector3_t lp_pos);
}