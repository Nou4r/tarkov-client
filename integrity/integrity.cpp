#include "integrity.hpp"

#include "../dependencies/xorstring/xorstring.hpp"

#include <string>
#include <random>
#include <vector>
#include <fstream>
#include <intrin.h>
#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

global_integrity integrity_intellisense;

char* integrity::utilities::get_data(std::string url)
{
	HINTERNET open, file = NULL;

	DWORD size, bytes = 0;

	open = InternetOpenA(NULL, NULL, NULL, NULL, NULL);
	if (!open)
		return NULL;

	char* c_str = new char[url.length() + 1];

	const char* header = "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.87 Safari/537.36\r\nSet-Cookie: BLAZINGFAST-WEB-PROTECT=15cea39a9a8a87b4b9496870e0351616; rcksid=PudWcrYLJzhg2nLHwyvXXx8n5Q0afe0yj1AHI4deuq6NTRJuTpbINaN8K2jikUNI;\r\nContent-Type: application/json\r\n\r\n\r\n";

	file = InternetOpenUrlA(open, strcpy(c_str, url.c_str()), header, strlen(header), INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE, NULL);

	if (!file)
	{
		InternetCloseHandle(open);
		return NULL;
	}

	char buffer[2000];

	if (InternetReadFile(file, buffer, _countof(buffer), &bytes))
	{
		buffer[bytes] = '\0';
	}
	else
		return NULL;

	InternetCloseHandle(file);
	InternetCloseHandle(open);
	char* ret = buffer;

	return ret;
}

auto integrity::utilities::generate_random_string(size_t length) -> std::string
{
	const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	std::string tmp_s;
	tmp_s.reserve(length);

	for (int i = 0; i < length; ++i) {
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	return tmp_s;
}

auto integrity::utilities::split_string(const std::string& str, const std::string& by) -> std::vector< std::string >
{
	std::vector<std::string> tokens;
	size_t prev = 0, pos = 0;

	do
	{
		pos = str.find(by, prev);
		if (pos == std::string::npos) pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + by.length();

	} while (pos < str.length() && prev < str.length());

	return tokens;
}

auto integrity::utilities::to_utf8(std::u16string utf16) -> std::string
{
	std::wstring_convert< std::codecvt_utf8_utf16< std::int16_t >, std::int16_t > wide_string;

	const auto conversion = reinterpret_cast<const std::int16_t*>(utf16.data());

	return wide_string.to_bytes(conversion, conversion + utf16.size());
}

void integrity::utilities::get_clipboard_data()
{
	OpenClipboard(nullptr);
	auto data = GetClipboardData(CF_TEXT);

	char* lock = static_cast<char*>(GlobalLock(data));
	std::string text(lock);

	GlobalUnlock(data); CloseClipboard();

	const auto split_structure = integrity::utilities::split_string(text, xs("_"));

	for (auto i = 0u; i < (int)split_structure.size(); ++i)
	{
		if ((int)split_structure.size() == 2)
		{
			integrity_intellisense.gpu.type = split_structure[0];

			integrity_intellisense.token = split_structure[1];
		}
	}
}

bool integrity::check()
{
	integrity::utilities::get_clipboard_data();

	std::string verify_token = utilities::get_data(xs("http://api.intellisense.club/request/stamp_query.php?stamp=") + integrity_intellisense.token);

	if (verify_token.find(integrity_intellisense.token) != std::string::npos)
	{
		return true;
	}
	else
		return false;

	return false;
}