#pragma once

#include <random>
#include <type_traits>

namespace modules::weapon::math
{
	int generate_random_number(const int minimum_number, const int maximum_number);
}

namespace modules::weapon::no_recoil
{
	void callback();
}