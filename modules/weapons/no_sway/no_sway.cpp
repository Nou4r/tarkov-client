#include "no_sway.hpp"

#include "../../../engine/engine.hpp"
#include "../../../renderer/menu/config/config.hpp"

void modules::weapon::no_sway::callback( )
{
	std::unique_lock<std::mutex> initiate_thread_locker( engine::values::global_locker );

	const auto camera_copy = engine::values::camera;

	const auto local_game_world_copy = engine::values::local_game_world;

	if ( !camera_copy.instance || !local_game_world_copy.instance )
		return;

	const auto local_player_copy = local_game_world_copy.get_local_player( );

	initiate_thread_locker.unlock( );

	if (std::get< bool >(config::config_map[features::enable_no_sway]))
	{
		if ( !local_player_copy.instance )
			return;

		const auto pwa = local_player_copy.get_procedural_weapon_animation();

		const auto local_player_breath_effector = pwa.get_breath_effector( );

		if ( !local_player_breath_effector.instance )
			return;

		pwa.get_motion_effector().set_intensity(0.f);

		local_player_breath_effector.set_sway_intensity( static_cast< float >( std::get< int >( config::config_map[features::sway_intensity] ) ) );
	}
}