#pragma once

#include <windows.h>
#include <string>

#include <codecvt>

#include <fstream>
#include <ctime>

#include <vector>

namespace integrity
{
	namespace utilities
	{
		void get_clipboard_data();

		std::vector< std::string > split_string(const std::string& str, const std::string& by);

		std::string to_utf8(std::u16string utf16);

		char* get_data(std::string url);

		std::string generate_random_string(size_t length);
	}

	bool check();
}

struct global_integrity
{
	std::string token;

	struct
	{
		std::string type;
	} gpu;
};

extern global_integrity integrity_intellisense;