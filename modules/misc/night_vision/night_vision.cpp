#include "night_vision.hpp"

#include "../../../engine/engine.hpp"

#include "../../../renderer/renderer.hpp"
#include "../../../renderer/menu/config/config.hpp"

void modules::misc::night_vision::callback()
{
	std::unique_lock<std::mutex> initiate_thread_locker(engine::values::global_locker);

	const auto camera_copy = engine::values::camera;

	const auto local_game_world_copy = engine::values::local_game_world;

	if (!camera_copy.instance || !local_game_world_copy.instance)
		return;

	initiate_thread_locker.unlock();

	if (const auto a1 = driver.read(camera_copy.instance + 0x30))
	{
		if (const auto list = driver.read(a1 + 0x30))
		{

			std::uintptr_t night_vision = 0u;

			if (!list)
				return;

			const auto size = driver.read<unsigned long>(list + 0xC);

			for (auto i = 0u; i < size * 0x10; i += 0x10)
			{
				if (const auto index = driver.read(list + i + 0x8))
				{
					const auto field = driver.read (index + 0x28);

					if(auto temp = driver.read(field))
					{
						if (temp = driver.read(temp))
						{
							if (temp = driver.read(temp + 0x48))
							{
								if (temp = driver.read(temp) == 0x736956746867694e)
								{
									night_vision = field;
								}
							}
						}
					}
				}
			}

			if (night_vision) 
			{
				driver.write< bool >(night_vision + 0xE4, std::get< bool >(config::config_map[features::enable_night_vision]));
				driver.write< float >(night_vision + 0xC0, .8f);
			}
		}
	}
}