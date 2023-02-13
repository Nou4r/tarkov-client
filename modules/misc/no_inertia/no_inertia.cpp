#include "no_inertia.hpp"

#include "../../../engine/engine.hpp"

#include "../../../renderer/renderer.hpp"
#include "../../../renderer/menu/config/config.hpp"

void modules::misc::no_inertia::callback( )
{
	std::unique_lock<std::mutex> initiate_thread_locker(engine::values::global_locker);

	const auto inertia_settings = engine::values::inertia_settings;

	initiate_thread_locker.unlock();

	const auto enabled = std::get< bool >(config::config_map[features::no_inertia]);

	if (inertia_settings)
	{
		driver.write<float>(inertia_settings + 0xF4, enabled ? 0.f : 0.25f);

		driver.write<float>(inertia_settings + 0xF0, enabled ? 0.f : 0.25f);

		driver.write<float>(inertia_settings + 0x58, enabled ? 0.f : 1.2300000190735f);

		driver.write<float>(inertia_settings + 0x54, enabled ? 0.f : 0.30000001192093f);

		driver.write<float>(inertia_settings + 0x50, enabled ? 0.f : 1.0700000524521f);

		driver.write<float>(inertia_settings + 0x4C, enabled ? 0.f : 0.40000000596046f);

		driver.write<float>(inertia_settings + 0x20, enabled ? 9999.f : 0.15000000596046f);
	}
}