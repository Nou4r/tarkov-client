#pragma once

#include <list>

#include "../../engine/engine.hpp"

static std::list< int > upper_part = { engine::structures::engine_bones::HumanHead };
static std::list< int > right_arm = { engine::structures::engine_bones::HumanHead, engine::structures::engine_bones::HumanRUpperarm, engine::structures::engine_bones::HumanRForearm1, engine::structures::engine_bones::HumanRDigit11 };
static std::list< int > left_arm = { engine::structures::engine_bones::HumanHead, engine::structures::engine_bones::HumanLUpperarm, engine::structures::engine_bones::HumanLForearm1, engine::structures::engine_bones::HumanLDigit11 };
static std::list< int > spine = { engine::structures::engine_bones::HumanHead, engine::structures::engine_bones::HumanSpine1, engine::structures::engine_bones::HumanSpine2 };

static std::list< int > lower_right = { engine::structures::engine_bones::HumanSpine2, engine::structures::engine_bones::HumanRThigh1, engine::structures::engine_bones::HumanRCalf, engine::structures::engine_bones::HumanRToe };
static std::list< int > lower_left = { engine::structures::engine_bones::HumanSpine2, engine::structures::engine_bones::HumanLThigh1, engine::structures::engine_bones::HumanLCalf , engine::structures::engine_bones::HumanLToe };

static std::list< std::list< int > > skeleton = { upper_part, right_arm, left_arm, spine, lower_right, lower_left };

namespace modules::visuals
{
	void callback( engine::types::camera_t camera_copy, engine::types::local_game_world_t local_game_world_copy, const std::vector<engine::types::player_t>& players_copy, engine::types::player_t local_player_copy, engine::types::player_t closest_player, engine::structures::vector3_t lp_pos);
}