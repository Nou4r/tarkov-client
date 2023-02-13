#include "engine.hpp"
#include "../settings/settings.hpp"
#include "../renderer/menu/config/config.hpp"
#include "../dependencies/mono/mono.hpp"

#include "types/types.hpp"

std::mutex engine::values::global_locker;
std::mutex engine::values::player_vector_locker;

std::mutex engine::values::loot_vector_locker;
std::mutex engine::values::grenade_vector_locker;

std::uintptr_t engine::values::unity_player = 0;
std::uintptr_t engine::values::mono_dll = 0;

engine::types::game_object_manager_t engine::values::game_object_manager{ 0};
engine::types::camera_t engine::values::camera{ 0};
engine::types::local_game_world_t engine::values::local_game_world{ 0 };

std::vector< engine::types::player_t > engine::values::players{};

std::vector< loot_structure_t > engine::values::loot{};
std::vector< grenade_structure_t > engine::values::grenades{};

engine::types::player_t engine::values::closest_player{};


std::mutex engine::values::aim_player_vector_locker;

std::mutex engine::values::closest_player_locker;

std::vector< engine::types::player_t > engine::values::aim_players;

engine::structures::vector3_t engine::values::velocity;

std::uintptr_t engine::values::hard_settings = 0;
std::uintptr_t engine::values::inertia_settings = 0;
std::uintptr_t engine::values::init_shot = 0;

std::once_flag setup_bases;
std::once_flag log_flag;

//func + 0xA3 = load ptr to vector into rcx

std::uint8_t jmp_shellcode[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
/*
0:  e9 00 00 00 00          jmp    5 <_main+0x5>
*/

std::uint8_t stub[] = { 0x48, 0xB9, 0xEF, 0xBE, 0xAD, 0xDE, 0xEF, 0xBE, 0xAD, 0xDE, 0x48, 0x8D, 0x86, 0xA8, 0x00, 0x00, 0x00 };
/*
0:  48 b9 ef be ad de ef    movabs rcx,0xdeadbeefdeadbeef load our vector at this ptr
7:  be ad de
a:  48 8d 86 a8 00 00 00    lea    rax,[rsi+0xa8]
*/

std::uint8_t starting_speed_spoof[] = { 0xF3, 0x0F, 0x10, 0x45, 0x40, 0x0F, 0x58, 0xC0, 0x0F, 0x58, 0xC0, 0x90, 0x90, 0xF3, 0x0F, 0x11, 0x86, 0xB4, 0x00, 0x00, 0x00 };
/*
0:  f3 0f 10 45 40          movss  xmm0,DWORD PTR [rbp+0x40]
5:  0f 58 c0                addps  xmm0,xmm0
8:  0f 58 c0                addps  xmm0,xmm0
b:  90                      nop
c:  90                      nop
d:  f3 0f 11 86 b4 00 00    movss  DWORD PTR [rsi+0xb4],xmm0
14: 00
*/

std::uint8_t speed_spoof[] = { 0xF3, 0x0F, 0x10, 0x45, 0x48, 0x0F, 0x58, 0xC0, 0x0F, 0x58, 0xC0, 0x90, 0x90, 0xF3, 0x0F, 0x11, 0x86, 0xB8, 0x00, 0x00, 0x00 };
/*
0:  f3 0f 10 45 48          movss  xmm0,DWORD PTR [rbp+0x48]
5:  0f 58 c0                addps  xmm0,xmm0
8:  0f 58 c0                addps  xmm0,xmm0
b:  90                      nop
c:  90                      nop
d:  f3 0f 11 86 b8 00 00    movss  DWORD PTR [rsi+0xb8],xmm0
14: 00
*/

static HWND game;

std::pair< std::int32_t, std::int32_t > engine::tools::get_screen_size()
{
	if (!game)
		game = lf(FindWindowA)(nullptr, xs("EscapeFromTarkov"));

	RECT screen_dimensions{ };

	if (lf(GetClientRect)(game, &screen_dimensions))
	{
		return { screen_dimensions.right - screen_dimensions.left, screen_dimensions.bottom - screen_dimensions.top };
	}

	return { 0, 0 };
}

void engine::functions::initiate( )
{
	while( true )
	{
		std::call_once( setup_bases, [=]()
			{
				engine::values::unity_player = driver.dll(L"UnityPlayer.dll");

				engine::values::mono_dll = driver.dll(xs(L"mono-2.0-bdwgc.dll"));

				while (!values::unity_player)
					engine::values::unity_player = driver.dll(L"UnityPlayer.dll");

				values::game_object_manager.instance = driver.read(engine::values::unity_player + engine::offsets::game_object_manager);				
			});

		if constexpr (settings::values::debug)
		{
			std::printf("unity_player %p\n", engine::values::unity_player);

			std::printf("game_object_manager %p\n", values::game_object_manager.instance);

			std::printf("base %p\n", driver.base());
		}

		const auto temporary_camera_value = engine::values::game_object_manager.get_camera_object_by_name( engine::types::name_hash_t::fps_camera ).game_object( ).class_object( ).get_object< types::camera_t >( );

		const auto temporary_local_game_world_value = driver.read< types::local_game_world_t >( engine::values::game_object_manager.get_active_object_by_name( engine::types::name_hash_t::game_world ).game_object( ).class_object( ).get_object( ) + engine::offsets::game_world::self );

		std::uintptr_t temp_hard_settings = 0u;
		std::uintptr_t temp_inertia_settings = 0u;

		if constexpr (settings::values::premium)
		{

			if (!engine::values::hard_settings)
			{
				if (const auto hard_s = mono::find_class("Assembly-CSharp", "EFTHardSettings"))
				{
					if (const auto method = hard_s->find_method("get_Instance"))
					{
						if (const auto method_addr_inst = mono::get_address_from_method(method))
						{
							if (const auto method_addr = driver.read(method_addr_inst + 0xA))
							{
								if (const auto addr = driver.read(method_addr))
								{
									temp_hard_settings = addr;
								}
							}
						}
					}

				}
			}//\uE62

			if (!engine::values::inertia_settings)
			{
				if (const auto hard_s = mono::find_class("Assembly-CSharp", "ProceduralWeaponAnimation"))
				{
					if (const auto method = hard_s->find_method("UpdateWeaponVariables"))
					{
						if (const auto method_addr_inst = mono::get_address_from_method(method))
						{
							if (const auto method_addr = driver.read(method_addr_inst + 0x27C)) //this is cringe but idc xd, the get_inertiaSettings doesnt get compiled cuz they use the singleton instance...
							{
								if (const auto addr = driver.read(method_addr))
								{
									if (const auto inertia_addr = driver.read(addr + 0xC0))
									{
										temp_inertia_settings = inertia_addr;
									}
								}
							}
						}
					}

				}
			}//\uE62

			if (!engine::values::init_shot)
			{
				std::uintptr_t unused_method = 0u;

				if (const auto hard_s = mono::find_class("Assembly-CSharp", "Player"))
				{
					if (const auto method = hard_s->find_method("Sleep"))
					{
						if (const auto method_addr_inst = mono::get_address_from_method(method))
						{
							std::printf( "found empty method %p\n", method_addr_inst );
							unused_method = method_addr_inst - 10;
						}
					}

				}

				if (unused_method)
				{
					if (const auto hard_s = mono::find_class("Assembly-CSharp", "\\uEA80")) //FormTrajectory
					{
						if (const auto method = hard_s->find_method("\\uE004"))
						{
							if (const auto method_addr_inst = mono::get_address_from_method(method))
							{
								engine::values::init_shot = method_addr_inst;

								const auto to_patch = method_addr_inst + 0xA3;

								*reinterpret_cast<std::uintptr_t*>(stub + 2) = driver.dll(xs(L"mono-2.0-bdwgc.dll")) + 0x4A33F0;
								driver.write_arr(unused_method, stub, sizeof(stub));

								*reinterpret_cast<std::int32_t*>(jmp_shellcode + 1) = (to_patch + 11) - (unused_method + sizeof(stub) + 5);
								driver.write_arr(unused_method + sizeof(stub), jmp_shellcode, sizeof(jmp_shellcode));

								*reinterpret_cast<std::int32_t*>(jmp_shellcode + 1) = (unused_method)-(to_patch + 5);
								driver.write_arr(to_patch, jmp_shellcode, sizeof(jmp_shellcode));

								//driver.write_arr(method_addr_inst + 0xC1, starting_speed_spoof, sizeof(starting_speed_spoof));

								//driver.write_arr(method_addr_inst + 0xd6, speed_spoof, sizeof(speed_spoof));
							}
						}

					}
				}
			}//\uE62
		}

		std::unique_lock<std::mutex> initiate_thread_locker( engine::values::global_locker );

		engine::values::camera = temporary_camera_value;

		engine::values::local_game_world = temporary_local_game_world_value;

		if (temp_hard_settings)
			engine::values::hard_settings = temp_hard_settings;

		if(temp_inertia_settings)
			engine::values::inertia_settings = temp_inertia_settings;

		initiate_thread_locker.unlock( );

		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	}
}

void engine::functions::update_closest_player()
{
	float distance = 9999.f;

	std::unique_lock<std::mutex> initiate_thread_locker(engine::values::global_locker);

	const auto camera_copy = engine::values::camera;

	const auto local_game_world_copy = engine::values::local_game_world;

	initiate_thread_locker.unlock();

	std::unique_lock<std::mutex> update_players_locker(engine::values::aim_player_vector_locker);
	const auto players = engine::values::aim_players;
	update_players_locker.unlock();

	if (!camera_copy.instance || !local_game_world_copy.instance)
		return;

	const auto local_player_copy = local_game_world_copy.get_local_player();

	types::player_t best_player = { 0 };

	for (const auto player : players)
	{
		if (!player.instance)
			continue;

		if (player.get_health_controller().get_health().first <= 0.f)
			continue;

		const auto head_world_position = player.get_bone_transform(engine::structures::bones::head).get_position();

		const auto root_world_position_lp = local_player_copy.get_bone_transform(engine::structures::bones::root).get_position();

		const auto aim_distance = static_cast<std::uint32_t>(head_world_position.distance(root_world_position_lp));

		if (aim_distance > std::get< int >(config::config_map[features::aimbot_max_distance]))
			continue;

		const auto head_position = camera_copy.world_to_screen(head_world_position);

		if (!head_position.has_value())
			continue;

		const auto head_screen_position = head_position.value();

		const auto w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);

		const auto screen_distance = std::sqrtf(std::powf(head_screen_position.x - w / 2, 2) + std::powf(head_screen_position.y - h / 2, 2));

		if (std::get< bool >(config::config_map[features::aimbot_show_fov]))
		{
			if (std::get<int>(config::config_map[features::aimbot_fov]) * 10.f < screen_distance)
				continue;
		}

		if (screen_distance < distance)
		{

			distance = screen_distance;
			best_player = player;
		}
	}

	std::unique_lock<std::mutex> lk(engine::values::closest_player_locker);
	engine::values::closest_player = best_player;
	lk.unlock();

	/*const auto fac = local_player_copy.get_procedural_weapon_animation().get_firearm_controller().instance;

	if (settings::values::premium && fac)
	{
		auto from = driver.read< engine::types::transform_t >(local_player_copy.instance, { 0x578, 0x148, 0x10 }).get_position();

		const auto gun_base = driver.read< engine::types::transform_t >(fac + 0xf8).get_position();

		auto head_world_position = engine::values::closest_player.get_bone_transform(engine::structures::bones::head).get_position();

		engine::values::velocity = engine::structures::vector3_t{ head_world_position - from }.normalized();
		const auto dir_ptr = engine::values::mono_dll + 0x4A33F0;
		driver.write(dir_ptr, engine::values::velocity);
	}*/
}

void engine::functions::update_players_vector( )
{
	while( true )
	{
		std::unique_lock<std::mutex> update_players_global_locker( engine::values::global_locker );

		if ( !engine::values::camera.instance || !engine::values::local_game_world.instance )
		{
			update_players_global_locker.unlock( );

			std::unique_lock<std::mutex> update_players_locker( engine::values::player_vector_locker );

			engine::values::players.clear( );

			update_players_locker.unlock( );

			continue;
		}

		types::local_game_world_t local_game_world_clone { engine::values::local_game_world.instance };

		update_players_global_locker.unlock( );

		const auto temporary_players = local_game_world_clone.get_players( );

		std::unique_lock<std::mutex> update_players_locker( engine::values::player_vector_locker );

		engine::values::players = temporary_players;

		update_players_locker.unlock();

		std::this_thread::sleep_for( std::chrono::milliseconds( 250 ) );
	}
}

void engine::functions::update_loot_vector( )
{
	while ( true )
	{
		std::unique_lock<std::mutex> update_items_global_locker( engine::values::global_locker );

		if ( !engine::values::camera.instance || !engine::values::local_game_world.instance )
		{
			update_items_global_locker.unlock( );

			std::unique_lock<std::mutex> update_items_locker( engine::values::loot_vector_locker );

			engine::values::loot.clear( );

			update_items_locker.unlock( );

			continue;
		}

		types::local_game_world_t local_game_world_clone { engine::values::local_game_world.instance };

		update_items_global_locker.unlock( );

		const auto temporary_items = local_game_world_clone.get_loot(local_game_world_clone.get_local_player());

		std::unique_lock<std::mutex> update_items_locker( engine::values::loot_vector_locker );

		engine::values::loot = temporary_items;

		update_items_locker.unlock();

		std::this_thread::sleep_for( std::chrono::seconds( 5 ) );
	}
}

void engine::functions::update_grenade_vector( )
{
	while ( true )
	{
		std::unique_lock<std::mutex> update_grenades_global_locker( engine::values::global_locker );

		if ( !engine::values::camera.instance || !engine::values::local_game_world.instance )
		{
			update_grenades_global_locker.unlock( );

			std::unique_lock<std::mutex> update_grenades_locker( engine::values::grenade_vector_locker );

			engine::values::grenades.clear( );

			update_grenades_locker.unlock( );

			continue;
		}

		types::local_game_world_t local_game_world_clone { engine::values::local_game_world.instance };

		update_grenades_global_locker.unlock( );

		const auto temporary_grenades = local_game_world_clone.get_grenades( local_game_world_clone.get_local_player( ) );

		std::unique_lock<std::mutex> update_grenades_locker( engine::values::loot_vector_locker );

		engine::values::grenades = temporary_grenades;

		update_grenades_locker.unlock();

		std::this_thread::sleep_for( std::chrono::milliseconds( 15 ) );
	}
}

void engine::functions::update_aim_players_vector()
{
	while( true )
	{
		std::unique_lock<std::mutex> update_players_global_locker( engine::values::global_locker );

		if ( !engine::values::camera.instance || !engine::values::local_game_world.instance )
		{
			update_players_global_locker.unlock( );

			std::unique_lock<std::mutex> update_players_locker( engine::values::aim_player_vector_locker);

			engine::values::players.clear( );

			update_players_locker.unlock( );

			continue;
		}

		types::local_game_world_t local_game_world_clone { engine::values::local_game_world.instance };

		update_players_global_locker.unlock( );

		const auto temporary_players = local_game_world_clone.get_players( );

		std::unique_lock<std::mutex> update_players_locker( engine::values::aim_player_vector_locker );
		
		engine::values::aim_players = temporary_players;
		
		update_players_locker.unlock( );

		std::this_thread::sleep_for( std::chrono::milliseconds( 250 ) );
	}
}
