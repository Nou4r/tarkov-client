#include "no_stamina.hpp"

#include "../../../engine/engine.hpp"

#include "../../../renderer/renderer.hpp"
#include "../../../renderer/menu/config/config.hpp"

void modules::misc::no_stamina::callback( )
{
	std::unique_lock<std::mutex> initiate_thread_locker( engine::values::global_locker );

	const auto camera_copy = engine::values::camera;

	const auto local_game_world_copy = engine::values::local_game_world;

	if ( !camera_copy.instance || !local_game_world_copy.instance )
		return;

	initiate_thread_locker.unlock( );

	if (local_game_world_copy.get_local_player( ).instance && std::get< bool >( config::config_map[features::enable_no_stamina] ) )
		local_game_world_copy.get_local_player( ).get_physical_controller( ).get_stamina( ).set( 1000.f );
}