#include "config.hpp"

#include "../menu.hpp"

std::variant< bool, int, float, engine::structures::vector4_t > config::config_map[]
{
	false,
	5,
	0,
	100,
	false,
	false,
	false,
	50,
	0,
	VK_RBUTTON,

	false,
	false,
	false,
	false,
	false,
	false,

	0,

	0,
	0,

	0,
	0,

	0,
	0,

	0,

	false,
	false,

	750,
	750,

	50000,
	5000,

	false,
	false,

	false,
	false,

	false,
	false,
	false,
	false,
	true,
	false,
	true,
	true,
	true,

	false,

	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },
	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },

	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },
	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },

	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },

	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },
	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },
	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },
	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },
	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },
	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },
	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },
	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },
	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },
	engine::structures::vector4_t { 0.f, 1.f, 0.f, 1.f },
	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },
};

void config::save( )
{
    std::ofstream config_file_path( xs( "C:\\Windows\\EFT_LITE.json" ) );
	
	nlohmann::json config_file;

	config_file[xs( "aimbot" )][xs( "enable" )] = std::get< bool >(config::config_map[features::aimbot_enable]);
	config_file[xs( "aimbot" )][xs( "hotkey" )] = std::get< int >( config::config_map[features::aimbot_hotkey] );
	config_file[xs( "aimbot" )][xs( "smoothness" )] = std::get< int >( config::config_map[features::aimbot_smoothness] );
	config_file[xs( "aimbot" )][xs( "bone" )] = std::get< int >( config::config_map[features::aimbot_bone] );
	config_file[xs( "aimbot" )][xs( "max_distance" )] = std::get< int >( config::config_map[features::aimbot_max_distance] );
	config_file[xs( "aimbot" )][xs( "ignore_ai" )] = std::get< bool >( config::config_map[features::aimbot_ignore_ai] );
	config_file[xs( "aimbot" )][xs( "snap_lines" )] = std::get< bool >( config::config_map[features::aimbot_snap_lines] );
	config_file[xs( "aimbot" )][xs( "show_fov" )] = std::get< bool >( config::config_map[features::aimbot_show_fov] );
	config_file[xs( "aimbot" )][xs( "fov" )] = std::get< int >( config::config_map[features::aimbot_fov] );
	config_file[xs( "aimbot" )][xs( "fov_color" )] = std::get< engine::structures::vector4_t >( config::config_map[features::fov_color] ).color;
	config_file[xs( "aimbot" )][xs( "fov_segments" )] = std::get< int >( config::config_map[features::aimbot_fov_segments] );

	config_file[xs( "miscellaneous" )][xs( "no_recoil" )] = std::get< bool >( config::config_map[features::enable_no_recoil] );
	config_file[xs( "miscellaneous" )][xs( "no_sway" )] = std::get< bool >( config::config_map[features::enable_no_sway] );
	config_file[xs( "miscellaneous" )][xs( "no_stamina" )] = std::get< bool >( config::config_map[features::enable_no_stamina] );
	config_file[xs( "miscellaneous" )][xs( "night_vision" )] = std::get< bool >( config::config_map[features::enable_night_vision] );
	config_file[xs( "miscellaneous" )][xs( "thermal_vision" )] = std::get< bool >( config::config_map[features::enable_thermal_vision] );
	config_file[xs( "miscellaneous" )][xs( "remove_visor" )] = std::get< bool >( config::config_map[features::enable_remove_visor] );

	config_file[xs( "miscellaneous" )][xs( "visor_intensity" )] = std::get< int >( config::config_map[features::visor_intensity] );

	config_file[xs( "miscellaneous" )][xs( "recoil_x" )] = std::get< int >( config::config_map[features::recoil_x] );
	config_file[xs( "miscellaneous" )][xs( "recoil_y" )] = std::get< int >( config::config_map[features::recoil_y] );
	config_file[xs( "miscellaneous" )][xs( "recoil_depth_x" )] = std::get< int >( config::config_map[features::recoil_depth_x] );
	config_file[xs( "miscellaneous" )][xs( "recoil_depth_y" )] = std::get< int >( config::config_map[features::recoil_depth_y] );
	config_file[xs( "miscellaneous" )][xs( "recoil_random_x" )] = std::get< int >( config::config_map[features::recoil_random_x] );
	config_file[xs( "miscellaneous" )][xs( "recoil_random_y" )] = std::get< int >( config::config_map[features::recoil_random_y] );

	config_file[xs( "miscellaneous" )][xs( "sway_intensity" )] = std::get< int >( config::config_map[features::sway_intensity] );

	config_file[xs( "visuals" )][xs( "display_max_distance" )] = std::get< int >( config::config_map[features::display_max_distance] );
	config_file[xs( "visuals" )][xs( "display_item_max_distance" )] = std::get< int >( config::config_map[features::display_item_max_distance] );

	config_file[xs( "visuals" )][xs( "display_item_max_price" )] = std::get< int >( config::config_map[features::display_item_max_price] );
	config_file[xs( "visuals" )][xs( "display_item_min_price" )] = std::get< int >( config::config_map[features::display_item_min_price] );

	config_file[xs( "visuals" )][xs( "display_extractions" )] = std::get< bool >( config::config_map[features::display_extractions] );
	config_file[xs( "visuals" )][xs( "extraction_color" )] = std::get< engine::structures::vector4_t >( config::config_map[features::extraction_color] ).color;

	config_file[xs( "visuals" )][xs( "display_items" )] = std::get< bool >( config::config_map[features::display_items] );
	config_file[xs( "visuals" )][xs( "item_color" )] = std::get< engine::structures::vector4_t >( config::config_map[features::item_color] ).color;
	config_file[xs( "visuals" )][xs( "expensive_item_color" )] = std::get< engine::structures::vector4_t >( config::config_map[features::expensive_item_color] ).color;

	config_file[xs( "visuals" )][xs( "display_corpses" )] = std::get< bool >( config::config_map[features::display_corpses] );
	config_file[xs( "visuals" )][xs( "corpse_color" )] = std::get< engine::structures::vector4_t >( config::config_map[features::corpse_color] ).color;

	config_file[xs( "visuals" )][xs( "display_grenades" )] = std::get< bool >( config::config_map[features::display_grenades] );
	config_file[xs( "visuals" )][xs( "grenade_color" )] = std::get< engine::structures::vector4_t >( config::config_map[features::grenade_color] ).color;

	config_file[xs( "visuals" )][xs( "display_bounding_box" )] = std::get< bool >( config::config_map[features::display_bounding_box] );
	config_file[xs( "visuals" )][xs( "bounding_box_color" )] = std::get< engine::structures::vector4_t >( config::config_map[features::bounding_box_color] ).color;
	
	config_file[xs( "visuals" )][xs( "display_filled_box" )] = std::get< bool >( config::config_map[features::display_filled_box] );
	config_file[xs( "visuals" )][xs( "filled_box_color" )] = std::get< engine::structures::vector4_t >( config::config_map[features::filled_box_color] ).color;
	
	config_file[xs( "visuals" )][xs( "display_corner_box" )] = std::get< bool >( config::config_map[features::display_corner_box] );
	config_file[xs( "visuals" )][xs( "corner_box_color" )] = std::get< engine::structures::vector4_t >( config::config_map[features::corner_box_color] ).color;
	
	config_file[xs( "visuals" )][xs( "display_head_hitbox" )] = std::get< bool >( config::config_map[features::display_head_hitbox] );
	config_file[xs( "visuals" )][xs( "hitbox_color" )] = std::get< engine::structures::vector4_t >( config::config_map[features::hitbox_color] ).color;

	config_file[xs( "visuals" )][xs( "display_players_name" )] = std::get< bool >( config::config_map[features::display_players_name] );
	config_file[xs( "visuals" )][xs( "player_names_color" )] = std::get< engine::structures::vector4_t >( config::config_map[features::player_names_color] ).color;

	config_file[xs( "visuals" )][xs( "display_players_type" )] = std::get< bool >( config::config_map[features::display_players_type] );
	config_file[xs( "visuals" )][xs( "player_types_color" )] = std::get< engine::structures::vector4_t >( config::config_map[features::player_types_color] ).color;
	
	config_file[xs( "visuals" )][xs( "display_skeleton" )] = std::get< bool >( config::config_map[features::display_skeleton] );
	config_file[xs( "visuals" )][xs( "skeleton_color" )] =  std::get< engine::structures::vector4_t >( config::config_map[features::skeleton_color] ).color; 
	
	config_file[xs( "visuals" )][xs( "display_distance" )] = std::get< bool >( config::config_map[features::display_distance] );
	config_file[xs( "visuals" )][xs( "distance_color" )] = std::get< engine::structures::vector4_t >( config::config_map[features::distance_color] ).color;
	
	config_file[xs( "visuals" )][xs( "display_health" )] = std::get< bool >( config::config_map[features::display_health] );
	config_file[xs( "visuals" )][xs( "health_color" )] = std::get< engine::structures::vector4_t >( config::config_map[features::health_color] ).color;

	config_file_path << config_file.dump( 4 );
	config_file_path.close( );
}

void config::load( )
{
	std::ifstream config_file_path( xs( "C:\\Windows\\EFT_LITE.json" ) );
	nlohmann::json config_file;
	config_file_path >> config_file;

	std::get< bool >( config::config_map[features::aimbot_enable] ) = config_file[xs( "aimbot" )][xs( "enable" )];
	std::get< int >( config::config_map[features::aimbot_hotkey] ) = config_file[xs( "aimbot" )][xs( "hotkey" )];
	std::get< int >( config::config_map[features::aimbot_smoothness] ) = config_file[xs( "aimbot" )][xs( "smoothness" )];
	std::get< int >( config::config_map[features::aimbot_bone] ) = config_file[xs( "aimbot" )][xs( "bone" )];
	std::get< int >( config::config_map[features::aimbot_max_distance] ) = config_file[xs( "aimbot" )][xs( "max_distance" )];
	std::get< bool >( config::config_map[features::aimbot_snap_lines] ) = config_file[xs( "aimbot" )][xs( "snap_lines" )];
	std::get< bool >( config::config_map[features::aimbot_show_fov] ) = config_file[xs( "aimbot" )][xs( "show_fov" )];
	std::get< int >( config::config_map[features::aimbot_fov] ) = config_file[xs( "aimbot" )][xs( "fov" )];
	std::get< engine::structures::vector4_t >( config::config_map[features::fov_color] ).color[0] = config_file[xs( "aimbot" )][xs( "fov_color" )][0];
	std::get< engine::structures::vector4_t >( config::config_map[features::fov_color] ).color[1] = config_file[xs( "aimbot" )][xs( "fov_color" )][1];
	std::get< engine::structures::vector4_t >( config::config_map[features::fov_color] ).color[2] = config_file[xs( "aimbot" )][xs( "fov_color" )][2];
	std::get< engine::structures::vector4_t >( config::config_map[features::fov_color] ).color[3] = config_file[xs( "aimbot" )][xs( "fov_color" )][3];
	std::get< int >( config::config_map[features::aimbot_fov_segments] ) = config_file[xs( "aimbot" )][xs( "fov_segments" )];

	std::get< bool >( config::config_map[features::enable_no_recoil] ) = config_file[xs( "miscellaneous")][xs( "no_recoil" )];
	std::get< bool >( config::config_map[features::enable_no_sway] ) = config_file[xs( "miscellaneous")][xs( "no_sway" )];
	std::get< bool >( config::config_map[features::enable_no_stamina] ) = config_file[xs( "miscellaneous")][xs( "no_stamina" )];
	std::get< bool >( config::config_map[features::enable_night_vision] ) = config_file[xs( "miscellaneous")][xs( "night_vision" )];
	std::get< bool >( config::config_map[features::enable_thermal_vision] ) = config_file[xs( "miscellaneous")][xs( "thermal_vision" )];
	std::get< bool >( config::config_map[features::enable_remove_visor] ) = config_file[xs( "miscellaneous")][xs( "remove_visor" )];

	std::get< int >( config::config_map[features::visor_intensity] ) = config_file[xs( "miscellaneous")][xs( "visor_intensity" )];

	std::get< int >( config::config_map[features::recoil_x] ) = config_file[xs( "miscellaneous")][xs( "recoil_x" )];
	std::get< int >( config::config_map[features::recoil_y] ) = config_file[xs( "miscellaneous")][xs( "recoil_y" )];
	std::get< int >( config::config_map[features::recoil_depth_x] ) = config_file[xs( "miscellaneous")][xs( "recoil_depth_x" )];
	std::get< int >( config::config_map[features::recoil_depth_y] ) = config_file[xs( "miscellaneous")][xs( "recoil_depth_y" )];
	std::get< int >( config::config_map[features::recoil_random_x] ) = config_file[xs( "miscellaneous")][xs( "recoil_random_x" )];
	std::get< int >( config::config_map[features::recoil_random_y] ) = config_file[xs( "miscellaneous")][xs( "recoil_random_y" )];

	std::get< int >( config::config_map[features::sway_intensity] ) = config_file[xs( "miscellaneous")][xs( "sway_intensity" )];

	std::get< int >( config::config_map[features::display_max_distance] ) = config_file[xs( "visuals" )][xs( "display_max_distance" )];
	std::get< int >( config::config_map[features::display_item_max_distance] ) = config_file[xs( "visuals")][xs( "display_max_distance" )]; 

	std::get< int >( config::config_map[features::display_item_max_price] ) = config_file[xs( "visuals")][xs( "display_item_max_price" )]; 
	std::get< int >( config::config_map[features::display_item_min_price] ) = config_file[xs( "visuals")][xs( "display_item_min_price" )]; 

	std::get< bool >( config::config_map[features::display_extractions] ) = config_file[xs( "visuals")][xs( "display_extractions" )]; 
	std::get< engine::structures::vector4_t >( config::config_map[features::extraction_color] ).color[0] = config_file[xs( "visuals" )][xs( "extraction_color" )][0];
	std::get< engine::structures::vector4_t >( config::config_map[features::extraction_color] ).color[1] = config_file[xs( "visuals" )][xs( "extraction_color" )][1];
	std::get< engine::structures::vector4_t >( config::config_map[features::extraction_color] ).color[2] = config_file[xs( "visuals" )][xs( "extraction_color" )][2];
	std::get< engine::structures::vector4_t >( config::config_map[features::extraction_color] ).color[3] = config_file[xs( "visuals" )][xs( "extraction_color" )][3];
	std::get< bool >( config::config_map[features::display_items] ) = config_file[xs( "visuals")][xs( "display_items" )]; 
	std::get< engine::structures::vector4_t >( config::config_map[features::item_color] ).color[0] = config_file[xs( "visuals" )][xs( "item_color" )][0];
	std::get< engine::structures::vector4_t >( config::config_map[features::item_color] ).color[1] = config_file[xs( "visuals" )][xs( "item_color" )][1];
	std::get< engine::structures::vector4_t >( config::config_map[features::item_color] ).color[2] = config_file[xs( "visuals" )][xs( "item_color" )][2];
	std::get< engine::structures::vector4_t >( config::config_map[features::item_color] ).color[3] = config_file[xs( "visuals" )][xs( "item_color" )][3];
	std::get< engine::structures::vector4_t >( config::config_map[features::expensive_item_color] ).color[0] = config_file[xs( "visuals" )][xs( "expensive_item_color" )][0];
	std::get< engine::structures::vector4_t >( config::config_map[features::expensive_item_color] ).color[1] = config_file[xs( "visuals" )][xs( "expensive_item_color" )][1];
	std::get< engine::structures::vector4_t >( config::config_map[features::expensive_item_color] ).color[2] = config_file[xs( "visuals" )][xs( "expensive_item_color" )][2];
	std::get< engine::structures::vector4_t >( config::config_map[features::expensive_item_color] ).color[3] = config_file[xs( "visuals" )][xs( "expensive_item_color" )][3];

	std::get< bool >( config::config_map[features::display_corpses] ) = config_file[xs( "visuals")][xs( "display_corpses" )]; 
	std::get< engine::structures::vector4_t >( config::config_map[features::corpse_color] ).color[0] = config_file[xs( "visuals" )][xs( "corpse_color" )][0];
	std::get< engine::structures::vector4_t >( config::config_map[features::corpse_color] ).color[1] = config_file[xs( "visuals" )][xs( "corpse_color" )][1];
	std::get< engine::structures::vector4_t >( config::config_map[features::corpse_color] ).color[2] = config_file[xs( "visuals" )][xs( "corpse_color" )][2];
	std::get< engine::structures::vector4_t >( config::config_map[features::corpse_color] ).color[3] = config_file[xs( "visuals" )][xs( "corpse_color" )][3];
	std::get< bool >( config::config_map[features::display_grenades] ) = config_file[xs( "visuals")][xs( "display_grenades" )]; 
	std::get< engine::structures::vector4_t >( config::config_map[features::grenade_color] ).color[0] = config_file[xs( "visuals" )][xs( "grenade_color" )][0];
	std::get< engine::structures::vector4_t >( config::config_map[features::grenade_color] ).color[1] = config_file[xs( "visuals" )][xs( "grenade_color" )][1];
	std::get< engine::structures::vector4_t >( config::config_map[features::grenade_color] ).color[2] = config_file[xs( "visuals" )][xs( "grenade_color" )][2];
	std::get< engine::structures::vector4_t >( config::config_map[features::grenade_color] ).color[3] = config_file[xs( "visuals" )][xs( "grenade_color" )][3];

	std::get< bool >( config::config_map[features::display_bounding_box] ) = config_file[xs( "visuals" )][xs( "display_bounding_box" )];
	std::get< engine::structures::vector4_t >( config::config_map[features::bounding_box_color] ).color[0] = config_file[xs( "visuals" )][xs( "bounding_box_color" )][0];
	std::get< engine::structures::vector4_t >( config::config_map[features::bounding_box_color] ).color[1] = config_file[xs( "visuals" )][xs( "bounding_box_color" )][1];
	std::get< engine::structures::vector4_t >( config::config_map[features::bounding_box_color] ).color[2] = config_file[xs( "visuals" )][xs( "bounding_box_color" )][2];
	std::get< engine::structures::vector4_t >( config::config_map[features::bounding_box_color] ).color[3] = config_file[xs( "visuals" )][xs( "bounding_box_color" )][3];
	std::get< bool >( config::config_map[features::display_filled_box] ) = config_file[xs( "visuals" )][xs( "display_filled_box" )];
	std::get< engine::structures::vector4_t >( config::config_map[features::filled_box_color] ).color[0] = config_file[xs( "visuals" )][xs( "filled_box_color" )][0];
	std::get< engine::structures::vector4_t >( config::config_map[features::filled_box_color] ).color[1] = config_file[xs( "visuals" )][xs( "filled_box_color" )][1];
	std::get< engine::structures::vector4_t >( config::config_map[features::filled_box_color] ).color[2] = config_file[xs( "visuals" )][xs( "filled_box_color" )][2];
	std::get< engine::structures::vector4_t >( config::config_map[features::filled_box_color] ).color[3] = config_file[xs( "visuals" )][xs( "filled_box_color" )][3];
	std::get< bool >( config::config_map[features::display_corner_box] ) = config_file[xs( "visuals" )][xs( "display_corner_box" )];
	std::get< engine::structures::vector4_t >( config::config_map[features::corner_box_color] ).color[0] = config_file[xs( "visuals" )][xs( "corner_box_color" )][0];
	std::get< engine::structures::vector4_t >( config::config_map[features::corner_box_color] ).color[1] = config_file[xs( "visuals" )][xs( "corner_box_color" )][1];
	std::get< engine::structures::vector4_t >( config::config_map[features::corner_box_color] ).color[2] = config_file[xs( "visuals" )][xs( "corner_box_color" )][2];
	std::get< engine::structures::vector4_t >( config::config_map[features::corner_box_color] ).color[3] = config_file[xs( "visuals" )][xs( "corner_box_color" )][3];
	std::get< bool >( config::config_map[features::display_head_hitbox] ) = config_file[xs( "visuals" )][xs( "display_head_hitbox" )];
	std::get< engine::structures::vector4_t >( config::config_map[features::hitbox_color] ).color[0] = config_file[xs( "visuals" )][xs( "hitbox_color" )][0];
	std::get< engine::structures::vector4_t >( config::config_map[features::hitbox_color] ).color[1] = config_file[xs( "visuals" )][xs( "hitbox_color" )][1];
	std::get< engine::structures::vector4_t >( config::config_map[features::hitbox_color] ).color[2] = config_file[xs( "visuals" )][xs( "hitbox_color" )][2];
	std::get< engine::structures::vector4_t >( config::config_map[features::hitbox_color] ).color[3] = config_file[xs( "visuals" )][xs( "hitbox_color" )][3];
	std::get< bool >( config::config_map[features::display_players_name] ) = config_file[xs( "visuals" )][xs( "display_players_name" )];
	std::get< engine::structures::vector4_t >( config::config_map[features::player_names_color] ).color[0] = config_file[xs( "visuals" )][xs( "player_names_color" )][0];
	std::get< engine::structures::vector4_t >( config::config_map[features::player_names_color] ).color[1] = config_file[xs( "visuals" )][xs( "player_names_color" )][1];
	std::get< engine::structures::vector4_t >( config::config_map[features::player_names_color] ).color[2] = config_file[xs( "visuals" )][xs( "player_names_color" )][2];
	std::get< engine::structures::vector4_t >( config::config_map[features::player_names_color] ).color[3] = config_file[xs( "visuals" )][xs( "player_names_color" )][3];
	std::get< bool >( config::config_map[features::display_players_type] ) = config_file[xs( "visuals" )][xs( "display_players_type" )];
	std::get< engine::structures::vector4_t >( config::config_map[features::player_types_color] ).color[0] = config_file[xs( "visuals" )][xs( "player_types_color" )][0];
	std::get< engine::structures::vector4_t >( config::config_map[features::player_types_color] ).color[1] = config_file[xs( "visuals" )][xs( "player_types_color" )][1];
	std::get< engine::structures::vector4_t >( config::config_map[features::player_types_color] ).color[2] = config_file[xs( "visuals" )][xs( "player_types_color" )][2];
	std::get< engine::structures::vector4_t >( config::config_map[features::player_types_color] ).color[3] = config_file[xs( "visuals" )][xs( "player_types_color" )][3];
	std::get< bool >( config::config_map[features::display_skeleton] ) = config_file[xs( "visuals" )][xs( "display_skeleton" )];
	std::get< engine::structures::vector4_t >( config::config_map[features::skeleton_color] ).color[0] = config_file[xs( "visuals" )][xs( "skeleton_color" )][0];
	std::get< engine::structures::vector4_t >( config::config_map[features::skeleton_color] ).color[1] = config_file[xs( "visuals" )][xs( "skeleton_color" )][1];
	std::get< engine::structures::vector4_t >( config::config_map[features::skeleton_color] ).color[2] = config_file[xs( "visuals" )][xs( "skeleton_color" )][2];
	std::get< engine::structures::vector4_t >( config::config_map[features::skeleton_color] ).color[3] = config_file[xs( "visuals" )][xs( "skeleton_color" )][3];
	std::get< bool >( config::config_map[features::display_distance] ) = config_file[xs( "visuals" )][xs( "display_distance" )];
	std::get< engine::structures::vector4_t >( config::config_map[features::distance_color] ).color[0] = config_file[xs( "visuals" )][xs( "distance_color" )][0];
	std::get< engine::structures::vector4_t >( config::config_map[features::distance_color] ).color[1] = config_file[xs( "visuals" )][xs( "distance_color" )][1];
	std::get< engine::structures::vector4_t >( config::config_map[features::distance_color] ).color[2] = config_file[xs( "visuals" )][xs( "distance_color" )][2];
	std::get< engine::structures::vector4_t >( config::config_map[features::distance_color] ).color[3] = config_file[xs( "visuals" )][xs( "distance_color" )][3];
	std::get< bool >( config::config_map[features::display_health] ) = config_file[xs( "visuals" )][xs( "display_health" )];
	std::get< engine::structures::vector4_t >( config::config_map[features::health_color] ).color[0] = config_file[xs( "visuals" )][xs( "health_color" )][0];
	std::get< engine::structures::vector4_t >( config::config_map[features::health_color] ).color[1] = config_file[xs( "visuals" )][xs( "health_color" )][1];
	std::get< engine::structures::vector4_t >( config::config_map[features::health_color] ).color[2] = config_file[xs( "visuals" )][xs( "health_color" )][2];
	std::get< engine::structures::vector4_t >( config::config_map[features::health_color] ).color[3] = config_file[xs( "visuals" )][xs( "health_color" )][3];
}