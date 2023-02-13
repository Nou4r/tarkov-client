#pragma once
#include <array>

namespace modules
{
	constexpr auto module_update_interval = 500u;

	using module_callback_t = void(*)();

	struct module_t
	{
		bool is_premium;

		module_callback_t callback;
	};

	extern std::array< module_t, 8 > module_array;

	void fire_modules( );
}
