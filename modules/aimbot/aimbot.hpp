#pragma once

#include "../../engine/engine.hpp"

namespace modules::aimbot
{
	namespace components
	{
		std::pair< engine::structures::vector2_t, engine::types::player_t > get_filtered_targets();
	}

	namespace visual
	{
		void draw_fov();
	}

	void callback();
}