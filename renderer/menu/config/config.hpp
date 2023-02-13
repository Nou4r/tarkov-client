#pragma once

#include <windows.h>
#include <string_view>
#include <unordered_map>

#include <variant>
#include <fstream>

#include "../../../dependencies/json/json.hpp"

#include "../../../dependencies/xorstring/xorstring.hpp"
#include "../../../engine/structures/structures.hpp"

enum features : int32_t
{
	aimbot_enable,
	aimbot_smoothness,
	aimbot_bone,
	aimbot_max_distance,
	aimbot_ignore_ai,
	aimbot_show_fov,
	aimbot_snap_lines,
	aimbot_fov,
	aimbot_fov_segments,
	aimbot_hotkey,

	enable_no_recoil,
	enable_no_sway,
	enable_no_stamina,
	enable_night_vision,
	enable_thermal_vision,
	enable_remove_visor,

	visor_intensity,

	recoil_x,
	recoil_y,

	recoil_depth_x,
	recoil_depth_y,

	recoil_random_x,
	recoil_random_y,

	sway_intensity,

	loot_through_walls,
	no_inertia,

	display_max_distance,
	display_item_max_distance,

	display_item_max_price,
	display_item_min_price,

	display_extractions,
	display_items,

	display_corpses,
	display_grenades,

	display_bounding_box,
	display_filled_box,
	display_players_name,
	display_players_type,
	display_corner_box,
	display_head_hitbox,
	display_skeleton,
	display_distance,
	display_health,

	hide_preview,

	extraction_color,

	item_color,
	expensive_item_color,

	corpse_color,
	grenade_color,

	bounding_box_color,
	filled_box_color,
	corner_box_color,
	player_names_color,
	player_types_color,
	hitbox_color,
	skeleton_color,
	distance_color,
	operator_color,
	health_color,
	fov_color,
};

namespace config
{
	extern std::variant< bool, int, float, engine::structures::vector4_t > config_map[];

	extern void save( );
	extern void load( );
}