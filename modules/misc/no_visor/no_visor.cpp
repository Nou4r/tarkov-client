#include "no_visor.hpp"

#include "../../../engine/engine.hpp"

#include "../../../renderer/renderer.hpp"
#include "../../../renderer/menu/config/config.hpp"

void modules::misc::no_visor::callback()
{
	std::unique_lock<std::mutex> initiate_thread_locker(engine::values::global_locker);

	const auto camera_copy = engine::values::camera;

	const auto local_game_world_copy = engine::values::local_game_world;

	if (!camera_copy.instance || !local_game_world_copy.instance)
		return;

	initiate_thread_locker.unlock();

	const auto list = driver.read(camera_copy.instance, { 0x30, 0x30 }); std::uintptr_t visor = 0u;

	const auto size = driver.read<unsigned long>(list + 0xC);

	for (auto i = 0u; i < size * 0x10; i += 0x10)
	{
		if (const auto index = driver.read(list + i + 0x8))
		{
			const auto field = driver.read (index + 0x28);

			if (auto temp = driver.read(field))
			{
				if (temp = driver.read(temp))
				{
					if (temp = driver.read(temp + 0x48))
					{
						if (temp = driver.read(temp) == 0x666645726f736956)
						{
							visor = field;
						}
					}
				}
			}
		}
	}

	if (visor && std::get< bool >(config::config_map[features::enable_remove_visor]))
		driver.write< float >(visor + 0xB8, static_cast<float>(std::get< int >(config::config_map[features::visor_intensity])));
}