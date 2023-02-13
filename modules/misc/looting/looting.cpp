#include "looting.hpp"

#include "../../../engine/engine.hpp"

#include "../../../renderer/renderer.hpp"
#include "../../../renderer/menu/config/config.hpp"

void modules::misc::looting::callback()
{
	std::unique_lock<std::mutex> initiate_thread_locker(engine::values::global_locker);

	const auto hard_settings = engine::values::hard_settings;

	initiate_thread_locker.unlock();

	const auto enabled = std::get< bool >(config::config_map[features::loot_through_walls]);

	if (hard_settings)
	{
		driver.write(hard_settings + 0x1C8, enabled ? 4.f : 1.2999999523163f);

		driver.write(hard_settings + 0x1CC, enabled ? 4.f : 1.0f);

		driver.write(hard_settings + 0x1D0, enabled ? -1.f : 0.15000000596046f);
	}
}