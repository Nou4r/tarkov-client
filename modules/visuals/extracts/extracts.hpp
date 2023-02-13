#pragma once

#include <list>

#include "../../../engine/engine.hpp"

namespace modules::visuals::extracts
{
	void callback(engine::types::camera_t camera_copy, engine::types::local_game_world_t local_game_world_copy, engine::types::player_t local_player_copy, engine::structures::vector3_t lp_pos);
}