#include "types.hpp"

#include "../engine.hpp"

#include "../../integrity/integrity.hpp"
#include "../../renderer/menu/config/config.hpp"

#include "../../dependencies/fnv1a/fnv1a.hpp"
#include "../../dependencies/json/json.hpp"

#include <fstream>
#include <mutex>

engine::types::object_t engine::types::game_object_manager_t::last_tagged_object() const
{
	return driver.read< engine::types::object_t >( instance + offsets::object::last_tagged_object );
}

engine::types::object_t engine::types::game_object_manager_t::first_tagged_object() const
{
	return driver.read< engine::types::object_t >( instance + offsets::object::first_tagged_object);
}

engine::types::object_t engine::types::game_object_manager_t::last_camera_tagged_object() const
{
	return driver.read< engine::types::object_t >( instance + offsets::object::last_camera_tagged_object);
}

engine::types::object_t engine::types::game_object_manager_t::first_camera_tagged_object() const
{
	return driver.read< engine::types::object_t >( instance + offsets::object::first_camera_tagged_object);
}

engine::types::object_t engine::types::game_object_manager_t::last_active_object() const
{
	return driver.read< engine::types::object_t >( instance + offsets::object::last_active_object);
}

engine::types::object_t engine::types::game_object_manager_t::first_active_object() const
{
	return driver.read< engine::types::object_t >( instance + offsets::object::first_active_object);
}

engine::types::class_object_t engine::types::game_object_t::class_object() const
{
	return driver.read< class_object_t >( instance + offsets::game_object::class_object );
}

engine::types::game_object_t engine::types::base_object_t::game_object() const
{
	return driver.read< game_object_t >( instance + offsets::base_object::game_object );
}

engine::types::base_object_t engine::types::base_object_t::next_base_object( ) const
{
	return driver.read< base_object_t >( instance + offsets::base_object::next_base_object );
}

std::vector< engine::types::player_t > engine::types::local_game_world_t::get_players( ) const
{
	std::vector< engine::types::player_t > players;

	if ( !instance )
		return players;

	const auto generic_list = driver.read( instance + engine::offsets::game_world::registered_players );

	const auto size = driver.read< std::uint32_t >(generic_list + offsets::generic_list::size );

	if ( !size )
		return players;

	const auto list = driver.read(generic_list + offsets::generic_list::elements ) + offsets::generic_list::first_element;

	for ( auto iterator = 1u; iterator < size; ++iterator )
	{
		const auto player = driver.read< engine::types::player_t >( list + ( iterator * sizeof( void* ) ) );

		if ( player.instance )
			players.push_back( player );
	}

	return players;
}

std::ifstream stream( std::string( "C:\\Windows\\Temp\\catalog.json" ) );
nlohmann::json java_stream = nlohmann::json::parse( stream );

std::vector< loot_structure_t > engine::types::local_game_world_t::get_loot(engine::types::player_t lp) const
{
	std::vector< loot_structure_t > loot { };

	std::string item_name = "NULL"; auto item_price = NULL;

	if ( !instance )
		return loot;

	const auto generic_list = driver.read( instance + engine::offsets::game_world::loot_list );

	const auto size = driver.read< std::uint32_t >( generic_list + offsets::generic_list::size );

	if ( !size )
		return loot;

	const auto list = driver.read( generic_list + offsets::generic_list::elements ) + offsets::generic_list::first_element;

	for ( auto iterator = 0u; iterator < size; ++iterator )
	{
        const auto item = driver.read< engine::types::loot_t >( list + ( iterator * sizeof( void* ) ) );

		const auto pos = item.get_position();

		const auto distance = static_cast<std::uint32_t>(pos.distance(lp.get_position()));

		if (distance > std::get< int >(config::config_map[features::display_item_max_distance]))
			continue;

		auto interactive_class = driver.read( item.get_item_object( ).instance + offsets::loot::interactive_class );
		
		auto item_class = driver.read( interactive_class + offsets::loot::item_class );
		
		auto item_profile = driver.read( item_class + offsets::loot::item_profile );

		auto item_cached_object = driver.read( item_profile + offsets::loot::item_cached_object );

		auto item_id = hash_constfnv64_string( driver.read_unicode_string( item_cached_object + offsets::loot::object_id, 24 ).c_str( ) );

		if ( item_id == hashc_constfnv64_string( "55d7217a4bdc2d86028b456d" ) ) // Corpse
			loot.push_back(loot_structure_t{ item_price, "Corpse", item.get_position() });
		else 
		{
			for ( auto i = 0u; i < java_stream["data"]["itemsByType"].size(); ++i )
			{
				if ( item.instance && !java_stream["data"]["itemsByType"][i]["id"].is_null( ) )
				{
					if ( hash_constfnv64_string( java_stream["data"]["itemsByType"][i]["id"].get< std::string >( ).c_str( ) ) != item_id )
						continue;

					item_name = java_stream["data"]["itemsByType"][i]["shortName"].get< std::string >( );

					const auto flee_price = java_stream["data"]["itemsByType"][i]["avg24hPrice"].get< int >();

					item_price = flee_price ? flee_price : java_stream["data"]["itemsByType"][i]["basePrice"].get< int >( );

					if(item_price <= std::get< int >(config::config_map[features::display_item_max_price]) && item_price >= std::get< int >(config::config_map[features::display_item_min_price]))
						loot.push_back(loot_structure_t{ item_price, item_name, pos });
				}
			}
		}	
	}

	return loot;
}

std::vector< grenade_structure_t > engine::types::local_game_world_t::get_grenades( engine::types::player_t lp ) const
{
	std::vector< grenade_structure_t > grenades{ };

	if ( !instance )
		return grenades;

	const auto grenade_class = driver.read( instance + engine::offsets::game_world::grenade_list );
	const auto grenade_list = driver.read( grenade_class + engine::offsets::generic_list::size );

	const auto generic_list = driver.read( grenade_list + engine::offsets::generic_list::elements );
	const auto generic_list_size = driver.read< std::uint32_t >( grenade_list + offsets::generic_list::size );

	if ( !generic_list_size )
		return grenades;

	for ( auto iterator = 0u; iterator < generic_list_size; ++iterator )
	{
        const auto grenade = driver.read< engine::types::grenade_t >( generic_list + engine::offsets::object::last_active_object + ( iterator * sizeof( void* ) ) );

		auto transform = driver.read< transform_t >( grenade.instance, { 0x10, 0x30, 0x30, 0x8, 0x28 } );

		const auto position = transform.get_position();

		const auto distance = static_cast<std::uint32_t>(position.distance(lp.get_position()));

		if (distance > std::get< int >(config::config_map[features::display_max_distance]))
			continue;

		grenades.push_back( grenade_structure_t{ position } );
	}

	return grenades;
}

engine::types::player_t engine::types::local_game_world_t::get_local_player( ) const
{
	if ( !instance )
		return { 0 };

	const auto generic_list = driver.read( instance + engine::offsets::game_world::registered_players );

	const auto size = driver.read< std::uint32_t >(generic_list + offsets::generic_list::size );

	if ( !size )
		return { 0 };

	const auto list = driver.read(generic_list + offsets::generic_list::elements ) + offsets::generic_list::first_element;

	if (!list)
		return { 0 };

	return driver.read< engine::types::player_t >( list );
}

engine::types::exfil_controller_t engine::types::local_game_world_t::get_exfil_controller() const
{
	return driver.read< engine::types::exfil_controller_t  >( instance + offsets::game_world::exfil_class );
}

engine::types::camera_t::view_matrix_t engine::types::camera_t::get_view_matrix() const
{
	return driver.read< engine::types::camera_t::view_matrix_t  >( instance + offsets::fps_camera::view_matrix );
}

void engine::types::shot_effector_t::set_recoil_strength_xy( const engine::structures::vector2_t recoil_strength_xy ) const
{
	driver.write( instance + offsets::shot_effector::recoil_strength_xy, recoil_strength_xy );
}

void engine::types::shot_effector_t::set_recoil_strength_z( const engine::structures::vector2_t recoil_strength_z ) const
{
	driver.write( instance + offsets::shot_effector::recoil_strength_z, recoil_strength_z );
}

void engine::types::breath_effector_t::set_sway_intensity( float intensity ) const
{
	driver.write( instance + offsets::breath_effector::intensity, intensity );
}

void engine::types::motion_effector_t::set_intensity(float intensity) const
{
	driver.write( instance + offsets::motion::intensity, intensity);
}

bool engine::types::breath_effector_t::is_aiming( ) const
{
	return driver.read< bool >( instance + offsets::breath_effector::is_aiming );
}

engine::types::shot_effector_t engine::types::procedural_weapon_animation_t::get_shot_effector( ) const
{
	return driver.read< shot_effector_t >(instance + offsets::procedural_weapon_animation::shootingg );
}

engine::types::breath_effector_t engine::types::procedural_weapon_animation_t::get_breath_effector( ) const
{
	return driver.read< engine::types::breath_effector_t >( instance + offsets::procedural_weapon_animation::breath );
}

engine::types::motion_effector_t engine::types::procedural_weapon_animation_t::get_motion_effector() const
{
	return driver.read< engine::types::motion_effector_t >(instance + offsets::procedural_weapon_animation::motion_react);
}

engine::types::weapon_t engine::types::firearm_controller_t::get_item() const
{
	if (!instance)
		return {};

	return driver.read< engine::types::weapon_t >(instance + 0x60);
}

std::string engine::types::weapon_t::get_name() const
{
	if (!instance)
		return {};

	const auto temp = driver.read<std::uintptr_t>(instance, { 0x40, 0x10 });

	if (!temp)
		return {};

	if (!driver.read<int32_t>(temp + 0x18))
		return {};


	return driver.read_unicode_string(temp + 0x14, driver.read<int32_t>(temp + 0x18));
}

engine::types::firearm_controller_t engine::types::procedural_weapon_animation_t::get_firearm_controller() const
{
	if (!instance)
		return {};

	return driver.read< engine::types::firearm_controller_t >(instance + 0x80);
}

engine::types::profile_t engine::types::player_t::get_profile_controller() const
{
	return driver.read< profile_t >( instance + offsets::player::profile );
}

engine::types::profile_infromation_t engine::types::profile_t::get_profile_information( ) const
{
	return driver.read< profile_infromation_t >( instance + engine::offsets::profile::information );
}

engine::types::procedural_weapon_animation_t engine::types::player_t::get_procedural_weapon_animation( ) const
{
	return driver.read< procedural_weapon_animation_t >( instance + offsets::player::procedural_weapon_animation );
}

engine::types::health_controller_t engine::types::player_t::get_health_controller() const
{
	return driver.read< health_controller_t >( instance + offsets::player::health_controller );
}

engine::types::physical_controller_t engine::types::player_t::get_physical_controller() const
{
	return driver.read< physical_controller_t >( instance + offsets::player::physical );
}

engine::structures::vector3_t engine::types::player_t::get_position() const
{
	return driver.read<engine::structures::vector3_t>(instance, { 0x10, 0x30, 0x30, 0x8, 0x38, 0x90 });
}

engine::types::stamina_t engine::types::physical_controller_t::get_stamina() const
{
	return driver.read< stamina_t >( instance + offsets::physical::stamina );
}

void engine::types::stamina_t::set( float value ) const
{
	driver.write( instance + offsets::physical::current_stamina, value );
}

std::string engine::types::profile_infromation_t::player_name() const
{
	const auto name = driver.read(instance + 0x10);

	const auto size = driver.read< int >(name + 0x10);

	return driver.read_unicode_string(name + 0x14, size);
}

std::string engine::types::profile_infromation_t::player_type() const
{
	if (driver.read<int32_t>(instance + 0x6C) > 0)
	{
		if (driver.read<int32_t>(instance + 0x68) == 4)
		{
			return xs("Player Scav");
		}
		else
			return xs("Player");
	}
	else
	{
		if (const auto settings = driver.read< std::uintptr_t >(instance + offsets::profile::settings))
		{
			if (auto role = driver.read< int >(settings + 0x10))
			{
				switch (role)
				{
				case engine::structures::player_types::marksman:
				{
					return xs("Marksman");
					break;
				}
				case engine::structures::player_types::scav:
				{
					return xs("Scav");
					break;
				}
				case engine::structures::player_types::cursed_scav:
				{
					return xs("Cursed Scav");
					break;
				}
				case engine::structures::player_types::boss_bully:
				{
					return xs("Boss Bully");
					break;
				}
				case engine::structures::player_types::bully_follower:
				{
					return xs("Bully Follower");
					break;
				}
				case engine::structures::player_types::boss_killa:
				{
					return xs("Boss Killa");
					break;
				}
				case engine::structures::player_types::boss_kojaniy:
				{
					return xs("Boss Kojaniy");
					break;
				}
				case engine::structures::player_types::kojaniy_follower:
				{
					return xs("Kojaniy Follower");
					break;
				}
				case engine::structures::player_types::pmc_bot:
				{
					return xs("PMC Bot");
					break;
				}
				case engine::structures::player_types::boss_gluhar:
				{
					return xs("Boss Gluhar");
					break;
				}
				case engine::structures::player_types::gluhar_assault:
				{
					return xs("Gluhar Assault");
					break;
				}
				case engine::structures::player_types::gluhar_security:
				{
					return xs("Gluhar Assault");
					break;
				}
				case engine::structures::player_types::gluhar_scout:
				{
					return xs("Gluhar Scout");
					break;
				}
				case engine::structures::player_types::gluhar_sniper:
				{
					return xs("Gluhar Sniper");
					break;
				}
				case engine::structures::player_types::boss_sanitar:
				{
					return xs("Boss Sanitar");
					break;
				}
				case engine::structures::player_types::follower_sanitar:
				{
					return xs("Sanitar Follower");
					break;
				}
				case engine::structures::player_types::sectant_warrior:
				{
					return xs("Sectant Warrior");
					break;
				}
				case engine::structures::player_types::sectant_priest:
				{
					return xs("Sectant Priest");
					break;
				}
				case engine::structures::player_types::boss_tagilla:
				{
					return xs("Boss Tagilla");
					break;
				}
				case engine::structures::player_types::follower_tagilla:
				{
					return xs("Tagilla Follower");
					break;
				}
				case engine::structures::player_types::boss_knight:
				{
					return xs("Boss Knight");
					break;
				}
				case engine::structures::player_types::follower_pipe:
				{
					return xs("Knight Follower");
					break;
				}
				case engine::structures::player_types::follower_bird:
				{
					return xs("Knight Follower");
					break;
				}
				case engine::structures::player_types::ex_usec:
				{
					return xs("Ex USEC");
					break;
				}
				case engine::structures::player_types::santa:
				{
					return xs("Santa");
					break;
				}
				default:
					return xs("Scav");
					break;
				}
			}
		}
		else
			return xs("Scav");
	}
}

std::pair<float, float> engine::types::health_controller_t::get_health() const
{
	if (const auto dictionary = driver.read< std::uintptr_t >(instance + 0x58))
	{

		if (const auto count = driver.read< std::uint32_t >(dictionary + 0x40))
		{

			const auto entry_start = driver.read< std::uintptr_t >(dictionary + 0x18);

			float current = 0.0f;
			float max = 0.f;

			for (auto i = 0u; i < count; ++i)
			{
				if (const auto body_part_state = driver.read< std::uintptr_t >(entry_start + 0x30 + 0x18 * i))
				{
					if (const auto health_wrapper = driver.read< std::uintptr_t >(body_part_state + 0x10))
					{

						current += driver.read< float >(health_wrapper + 0x10);

						max += driver.read< float >(health_wrapper + 0x14);
					}
				}
			}

			return { current, max };
		}
	}

	return { 0.f, 0.f };
}

constexpr auto M_PI = 3.14159265358979323846f;

std::optional<engine::structures::vector2_t> engine::types::camera_t::world_to_screen(const engine::structures::vector3_t& position) const
{
	const auto width = GetSystemMetrics(SM_CXSCREEN), height = GetSystemMetrics(SM_CYSCREEN);

	std::unique_lock<std::mutex> initiate_thread_locker(engine::values::global_locker);

	const auto game_object_manager_copy = engine::values::game_object_manager;

	const auto camera_copy = engine::values::camera;

	const auto local_game_world_copy = engine::values::local_game_world;

	if (!camera_copy.instance || !local_game_world_copy.instance)
		return std::nullopt;

	const auto local_player_copy = local_game_world_copy.get_local_player();

	initiate_thread_locker.unlock();

	const auto last_camera_object = game_object_manager_copy.last_tagged_object( ).as< types::base_object_t >( );

	const auto current_game_object = last_camera_object.game_object( );

	const auto current_class_object = current_game_object.class_object( );

	auto view_matrix = local_player_copy.get_procedural_weapon_animation().get_breath_effector().is_aiming() && current_game_object.is( types::name_hash_t::optical_camera ) ? current_class_object.get_object< types::camera_t >().get_view_matrix() : get_view_matrix();

	const auto first = engine::structures::vector3_t{ view_matrix._11, view_matrix._21, view_matrix._31 };
	const auto second = engine::structures::vector3_t{ view_matrix._12, view_matrix._22, view_matrix._32 };
	const auto fourth = engine::structures::vector3_t{ view_matrix._14, view_matrix._24, view_matrix._34 };

	float y = second.dot(position) + view_matrix._42;
	float x = first.dot(position) + view_matrix._41;

	if ( local_player_copy.instance && local_player_copy.get_procedural_weapon_animation().get_breath_effector().is_aiming() && current_game_object.is( types::name_hash_t::optical_camera ) )
	{
		x /= cos((M_PI / 180) * driver.read<float>(this->instance + 0x15C) * 0.5f) / sin((M_PI / 180) * driver.read<float>(this->instance + 0x15C) * 0.5f) * driver.read<float>(this->instance + 0x4C8 ) * 0.5f;
		y /= cos((M_PI / 180) * driver.read<float>(this->instance + 0x15C) * 0.5f) / sin((M_PI / 180) * driver.read<float>(this->instance + 0x15C) * 0.5f) * 0.5f;
	}

	const auto w = fourth.dot( position ) + view_matrix._44;

	if ( w < 0.098f )
		return std::nullopt;

	const auto pos = engine::structures::vector2_t{ (1.f + (x) / w) * (width / 2), (1.f - (y) / w) * (height / 2) };

	if (pos.x > 0 && pos.x <= static_cast<float>(width) && pos.y > 0 && pos.y <= static_cast<float>(height))
		return pos;

	return std::nullopt;
}

std::vector<engine::types::base_object_t> engine::types::game_object_manager_t::get_tagged_objects() const
{
	std::vector< base_object_t > tagged_objects;

	auto last_tagged_object = this->last_tagged_object( ).as< types::base_object_t >( );

	if( !last_tagged_object.instance )
		return tagged_objects;

	auto current_tagged_object = types::base_object_t{ driver.read<std::uintptr_t>( this->first_tagged_object( ).instance ) };

	while( current_tagged_object.instance && current_tagged_object.game_object().instance && current_tagged_object.game_object( ).instance != last_tagged_object.game_object().instance )
	{
		if( current_tagged_object.instance )
			tagged_objects.push_back(current_tagged_object.as< types::base_object_t >( ) );

		current_tagged_object.instance = current_tagged_object.next_base_object( ).instance;
	}

	if( last_tagged_object.instance )
		tagged_objects.push_back( last_tagged_object );

	return tagged_objects;
}

std::vector<engine::types::base_object_t> engine::types::game_object_manager_t::get_camera_objects( ) const
{
	std::vector< base_object_t > camera_objects;

	auto last_camera_object = this->last_camera_tagged_object( ).as< types::base_object_t >( );

	if( !last_camera_object.instance )
		return camera_objects;

	auto current_camera_object = types::base_object_t{ driver.read<std::uintptr_t>( this->first_camera_tagged_object( ).instance ) };

	while( current_camera_object.instance && current_camera_object.game_object().instance && current_camera_object.game_object( ).instance != last_camera_object.game_object( ).instance )
	{
		if( current_camera_object.instance )
			camera_objects.push_back(current_camera_object.as< types::base_object_t >( ) );

		current_camera_object.instance = current_camera_object.next_base_object( ).instance;
	}

	if( last_camera_object.instance )
		camera_objects.push_back( last_camera_object);

	return camera_objects;
}

std::vector<engine::types::base_object_t> engine::types::game_object_manager_t::get_active_objects() const
{
	std::vector< base_object_t > active_objects;

	auto last_active_object = this->last_active_object( ).as< types::base_object_t >( );

	if( !last_active_object.instance )
		return active_objects;

	auto current_active_object = types::base_object_t{ driver.read<std::uintptr_t>( this->first_active_object( ).instance ) };

	while( current_active_object.instance && current_active_object.game_object().instance && current_active_object.game_object( ).instance != last_active_object.game_object( ).instance )
	{
		if( current_active_object.instance )
			active_objects.push_back( current_active_object.as< types::base_object_t >( ) );

		current_active_object.instance = current_active_object.next_base_object( ).instance;
	}

	if( last_active_object.instance )
		active_objects.push_back( last_active_object );

	return active_objects;
}

engine::types::base_object_t engine::types::game_object_manager_t::get_tagged_object_by_name( const name_hash_t name ) const
{
	auto last_tagged_object = this->last_tagged_object( ).as< types::base_object_t >() ;

	if( !last_tagged_object.instance )
		return { 0 };

	auto current_tagged_object = types::base_object_t{ driver.read<std::uintptr_t>( this->last_tagged_object( ).instance ) };

	while( current_tagged_object.instance && current_tagged_object.game_object( ).instance != last_tagged_object.instance )
	{
		if( current_tagged_object.instance )
		{
			const auto game_object = current_tagged_object.game_object( );

			if ( game_object.instance && game_object.is( name ) )
			{
				return current_tagged_object;
			}
		}

		current_tagged_object.instance = current_tagged_object.next_base_object( ).instance;
	}

	if( last_tagged_object.instance )
	{
		const auto game_object = last_tagged_object.game_object( );

		if ( game_object.instance && game_object.is( name ) )
		{
			return last_tagged_object.as< base_object_t >( );
		}
	}

	return { 0 };
}

engine::types::base_object_t engine::types::game_object_manager_t::get_camera_object_by_name( const name_hash_t name ) const
{
	auto last_camera_object = this->last_camera_tagged_object( ).as< types::base_object_t >( );

	if( !last_camera_object.instance )
		return { 0 };

	auto current_camera_object = types::base_object_t{ driver.read<std::uintptr_t>( this->first_camera_tagged_object( ).instance ) };

	while( current_camera_object.instance && current_camera_object.game_object( ).instance && current_camera_object.game_object( ).instance != last_camera_object.game_object( ).instance )
	{
		if( current_camera_object.instance )
		{
			const auto game_object = current_camera_object.game_object();

			if ( game_object.instance && game_object.is( name ) )
			{
				return current_camera_object;
			}
		}

		current_camera_object.instance = current_camera_object.next_base_object( ).instance;
	}

	if( last_camera_object.instance )
	{
		const auto game_object = last_camera_object.game_object( );

		if ( game_object.instance && game_object.is( name ) )
		{
			return last_camera_object.as< base_object_t >( );
		}
	}

	return { 0 };
}

engine::types::base_object_t engine::types::game_object_manager_t::get_active_object_by_name( const name_hash_t name ) const
{
	auto last_active_object = this->last_active_object( ).as< types::base_object_t >( );

	if( !last_active_object.instance )
		return { 0 };

	auto current_active_object = types::base_object_t{ driver.read<std::uintptr_t>( this->first_active_object( ).instance ) };

	while( current_active_object.instance && current_active_object.game_object().instance && current_active_object.game_object( ).instance != last_active_object.game_object( ).instance )
	{
		if( current_active_object.instance )
		{
			const auto game_object = current_active_object.game_object( );

			if ( game_object.instance && game_object.is( name ) )
			{
				return current_active_object;
			}
		}

		current_active_object.instance = current_active_object.next_base_object( ).instance;
	}

	if( last_active_object.instance )
	{
		const auto game_object = last_active_object.game_object( );

		if (game_object.instance && game_object.is(name))
		{
			return last_active_object.as< base_object_t >( );
		}
	}

	return { 0 };
}

engine::structures::vector3_t engine::types::transform_t::get_position() const
{
	auto transform_internal = driver.read<uint64_t>(instance + 0x10);

	if (!transform_internal)
		return engine::structures::vector3_t{ 0, 0, 0 };

	auto some_ptr = driver.read<uint64_t>(transform_internal + 0x38);
	auto index = driver.read<int32_t>(transform_internal + 0x38 + sizeof(uint64_t));
	if (!some_ptr)
		return engine::structures::vector3_t();

	auto relation_array = driver.read<uint64_t>(some_ptr + 0x18);
	if (!relation_array)
		return engine::structures::vector3_t();

	auto dependency_index_array = driver.read<uint64_t>(some_ptr + 0x20);
	if (!dependency_index_array)
		return engine::structures::vector3_t();

	__m128i temp_0;
	__m128 xmmword_1410D1340 = { -2.f, 2.f, -2.f, 0.f };
	__m128 xmmword_1410D1350 = { 2.f, -2.f, -2.f, 0.f };
	__m128 xmmword_1410D1360 = { -2.f, -2.f, 2.f, 0.f };
	__m128 temp_1;
	__m128 temp_2;
	auto temp_main = driver.read<__m128>(relation_array + index * 48);
	auto dependency_index = driver.read<int32_t>(dependency_index_array + 4 * index);

	int failsafe = 0;

	while (dependency_index >= 0) {
		auto relation_index = 6 * dependency_index;

		if (++failsafe > 50)
		{
			std::printf("hit failsafe\n");

			break;
		}

		temp_0 = driver.read<__m128i>(relation_array + 8 * relation_index + 16);
		temp_1 = driver.read<__m128>(relation_array + 8 * relation_index + 32);
		temp_2 = driver.read<__m128>(relation_array + 8 * relation_index);

		__m128 v10 = _mm_mul_ps(temp_1, temp_main);
		__m128 v11 = _mm_castsi128_ps(_mm_shuffle_epi32(temp_0, 0));
		__m128 v12 = _mm_castsi128_ps(_mm_shuffle_epi32(temp_0, 85));
		__m128 v13 = _mm_castsi128_ps(_mm_shuffle_epi32(temp_0, -114));
		__m128 v14 = _mm_castsi128_ps(_mm_shuffle_epi32(temp_0, -37));
		__m128 v15 = _mm_castsi128_ps(_mm_shuffle_epi32(temp_0, -86));
		__m128 v16 = _mm_castsi128_ps(_mm_shuffle_epi32(temp_0, 113));
		__m128 v17 = _mm_add_ps(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_sub_ps(
							_mm_mul_ps(_mm_mul_ps(v11, xmmword_1410D1350), v13),
							_mm_mul_ps(_mm_mul_ps(v12, xmmword_1410D1360), v14)),
						_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v10), -86))),
					_mm_mul_ps(
						_mm_sub_ps(
							_mm_mul_ps(_mm_mul_ps(v15, xmmword_1410D1360), v14),
							_mm_mul_ps(_mm_mul_ps(v11, xmmword_1410D1340), v16)),
						_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v10), 85)))),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_sub_ps(
							_mm_mul_ps(_mm_mul_ps(v12, xmmword_1410D1340), v16),
							_mm_mul_ps(_mm_mul_ps(v15, xmmword_1410D1350), v13)),
						_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v10), 0))),
					v10)),
			temp_2);

		temp_main = v17;
		dependency_index = driver.read<int32_t>(dependency_index_array + 4 * dependency_index);
	}

	return *reinterpret_cast<engine::structures::vector3_t*>(&temp_main);
}

std::uint64_t engine::types::item_cache_t::get_id( ) const
{
	return driver.read< std::uint64_t >( instance + 0x14 );
}

engine::types::item_cache_t engine::types::item_profile_t::get_cached_ptr( ) const
{
	return driver.read< item_cache_t >( instance + 0x50 );
}

engine::types::item_profile_t engine::types::item_class_t::get_item_profile( ) const
{
	return driver.read< item_profile_t >( instance + 0x40 );
}

engine::types::item_class_t engine::types::item_interactive_t::get_item_class( ) const
{
	return driver.read< item_class_t >( instance + 0x50 );
}

engine::types::item_interactive_t engine::types::item_object_t::get_item_interactive( ) const
{
	return driver.read< item_interactive_t >( instance + 0x28 );
}

engine::types::item_object_t engine::types::loot_t::get_item_object( ) const
{
	return driver.read< item_object_t >( instance + 0x10 );
}

engine::structures::vector3_t engine::types::loot_t::get_position( ) const
{
	const auto item_object = driver.read( instance + 0x10 );

	auto item_profile = driver.read( item_object + 0x28 );

	if ( item_profile )
	{
		const auto item_temporary = driver.read( item_object + 0x30 );

		auto relative_transform = driver.read( item_temporary + 0x30 );

		auto actual_transform = driver.read( relative_transform + 0x8 );

		auto position = driver.read( actual_transform + 0x38 );

		auto coordinates = driver.read< structures::vector3_t >( position + 0x90 );

		if ( !coordinates.x || !coordinates.y )
			return { };

		return coordinates;
	}

	return { };
}

engine::types::transform_t engine::types::player_t::get_bone_transform( engine::structures::bones bone ) const
{
	if (!instance)
		return { 0 };

	if( const auto body = driver.read(instance + engine::offsets::player::player_body ) )
	{
		if ( auto temp = driver.read(body + 0x28 ) ) //diz.skinning.skeleton
		{
			if ( temp = driver.read(temp + 0x28) ) // _values generic_list<transform>
			{
				const auto size = driver.read< std::uint32_t >(temp + offsets::generic_list::size);

				if (!size)
					return {0};

				if ( const auto bone_array = driver.read(temp + offsets::generic_list::elements) )
				{
					return driver.read< engine::types::transform_t >( bone_array + offsets::generic_list::first_element + bone * 8);
				}
			}
		}
	}

	return { 0 };
}

void engine::types::player_t::get_bone_transforms( transform_t* buff ) const
{

	if (const auto body = driver.read(instance + engine::offsets::player::player_body))
	{
		if (auto temp = driver.read(body + 0x28)) //diz.skinning.skeleton
		{
			if (temp = driver.read(temp + 0x28)) // _values generic_list<transform>
			{
				const auto size = driver.read< std::uint32_t >(temp + offsets::generic_list::size);

				if (!size)
					return;

				if (const auto bone_array = driver.read(temp + offsets::generic_list::elements))
				{

					driver.read_arr< transform_t >(bone_array + 32, buff, 134);
				}
			}
		}
	}
}

std::vector< std::pair<engine::types::exfil_t, std::string>> engine::types::exfil_controller_t::get_exfils( ) const
{
	std::vector< std::pair<engine::types::exfil_t, std::string>> exfils;

	if (!instance)
		return exfils;

	auto exfil_list = driver.read( instance + engine::offsets::exfil_class::exfil_points );

	if (!exfil_list)
		return exfils;

	auto count = driver.read<std::uint32_t>( exfil_list + engine::offsets::generic_list::size );

	for (auto i = 0u; i < count; ++i)
	{
		const auto exit = driver.read< engine::types::exfil_t >( exfil_list + engine::offsets::generic_list::first_element + ( i * sizeof(std::uintptr_t) ) );

		if (exit.instance)
		{
			const auto settings = driver.read(exit.instance + 0x58);
			if (!settings)
				continue;

			const auto name = driver.read(settings + 0x10);
			if (!name)
				continue;

			exfils.emplace_back(exit, driver.read_unicode_string(name + 0x14, driver.read<std::uint32_t>(name + 0x10)));

		}
	}

	return exfils;
}