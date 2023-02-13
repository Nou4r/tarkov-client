#pragma once
#include <Windows.h>

#include "../../driver/driver.hpp"
#include <locale>
#include <codecvt>

/*https://raw.githubusercontent.com/reahly/mono-external-lib/main/mono.hh*/

#define OFFSET(func, type, offset) type func { return driver.read<type>( reinterpret_cast<uintptr_t>( this ) + offset ); }  // NOLINT

struct glist_t {
	OFFSET(data(), uintptr_t, 0x0)
		OFFSET(next(), uintptr_t, 0x8)
};

struct mono_root_domain_t {
	OFFSET(domain_assemblies(), glist_t*, 0xC8)
};

struct mono_table_info_t {
	int get_rows() {
		return driver.read<int>(reinterpret_cast<uintptr_t>(this) + 0x8) & 0xFFFFFF;
	}
};

struct mono_jit_hash_table_t {
	OFFSET(size(), int, 0x18)
	OFFSET(data(), uintptr_t, 0x20)
	OFFSET(next_value(), void*, 0x8)
	OFFSET(key_extract(), void*, 0x0)

	std::uint64_t transform_key(unsigned int a1)
	{
		return 0x9E3779B1 * (a1 >> 3);
	}

	template<typename T>
	T lookup(void* key) {
		auto v4 = static_cast<mono_jit_hash_table_t*>(driver.read<void*>(data() + 0x8 * (transform_key(reinterpret_cast<std::uintptr_t>(key)) % this->size())));

		if (!v4)
			return {};

		while (v4->key_extract() != key) {
			v4 = static_cast<mono_jit_hash_table_t*>(v4->next_value());
			if (!v4)
				return {};
		}

		return reinterpret_cast<T>(v4);
	}
};

inline std::uint16_t utf8_to_utf16(const char* utf8) {
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
	std::u16string dest = convert.from_bytes(utf8);
	return *reinterpret_cast<std::uint16_t*>(&dest[0]);
}

struct mono_method_t {
	std::string name() {
		char data[128];

		driver.read_arr(driver.read(reinterpret_cast<std::uint8_t*>(this) + 0x18), data, 128);

		if (static_cast<std::uint8_t>(data[0]) == 0xEE)
		{
			char buff[32];
			sprintf_s(buff, 32, "\\u%04X", utf8_to_utf16(data));

			return buff;
		}

		return driver.read_string(reinterpret_cast<std::uintptr_t>(this) + 0x18);
	}
};

struct mono_class_field_t {
	OFFSET(offset(), int, 0x18)

		std::string name() {
		return driver.read_string(reinterpret_cast<std::uintptr_t>(this) + 0x8);
	}
};

struct mono_class_t {
	OFFSET(num_fields(), int, 0x100)

	std::string name() {
		char data[128];

		driver.read_arr(driver.read(reinterpret_cast<std::uint8_t*>(this) + 0x48), data, 128);

		if (static_cast<std::uint8_t>(data[0]) == 0xEE)
		{
			char buff[32];
			sprintf_s(buff, 32, "\\u%04X", utf8_to_utf16(data));

			return buff;
		}

		return driver.read_string(reinterpret_cast<std::uintptr_t>(this) + 0x48);
	}

	int get_num_methods() {
		const auto v2 = (driver.read<int>(reinterpret_cast<std::uint8_t*>(this) + 0x2a) & 7) - 1;
		switch (v2) {
		case 0:
		case 1:
			return driver.read<int>(reinterpret_cast<std::uint8_t*>(this) + 0xFC);

		case 3:
		case 5:
			return 0;

		case 4u:
			return driver.read<int>(reinterpret_cast<std::uint8_t*>(this) + 0xF0);

		default: break;
		}

		return 0;
	}

	mono_method_t* get_method(const int i) {
		return reinterpret_cast<mono_method_t*>(driver.read<uintptr_t>(driver.read<uintptr_t>(reinterpret_cast<uintptr_t>(this) + 0xA0) + 0x8 * i));
	}

	mono_class_field_t* get_field(const int i) {
		return reinterpret_cast<mono_class_field_t*>(driver.read<uintptr_t>(reinterpret_cast<uintptr_t>(this) + 0x98) + 0x20 * i);
	}

	mono_method_t* find_method(const char* method_name) {
		for (auto i = 0; i < this->get_num_methods(); i++) {
			const auto method = this->get_method(i);
			if (!method)
				continue;

			if (!strcmp(method->name().c_str(), method_name))
				return method;
		}

		return nullptr;
	}

	mono_class_field_t* find_field(const char* field_name) {
		for (auto i = 0; i < this->num_fields(); i++) {
			const auto field = this->get_field(i);
			if (!field)
				continue;

			if (!strcmp(field->name().c_str(), field_name))
				return field;
		}

		return nullptr;
	}
};

struct mono_hash_table_t {
	OFFSET(size(), int, 0x18)
		OFFSET(data(), uintptr_t, 0x20)
		OFFSET(next_value(), void*, 0x108)
		OFFSET(key_extract(), unsigned int, 0x58)

		template<typename T>
	T* lookup(void* key) {
		auto v4 = static_cast<mono_hash_table_t*>(driver.read<void*>(data() + 0x8 * (reinterpret_cast<unsigned int>(key) % this->size())));
		if (!v4)
			return nullptr;

		while (reinterpret_cast<void*>(v4->key_extract()) != key) {
			v4 = static_cast<mono_hash_table_t*>(v4->next_value());
			if (!v4)
				return nullptr;
		}

		return reinterpret_cast<T*>(v4);
	}
};

struct mono_image_t {
	OFFSET(flags(), int, 0x1C)

		mono_table_info_t* get_table_info(const int table_id) {
		if (table_id > 55)
			return nullptr;

		return reinterpret_cast<mono_table_info_t*>(reinterpret_cast<uintptr_t>(this) + 0x10 * (static_cast<int>(table_id) + 0xE));
	}

	mono_class_t* get(const int type_id) {
		if ((this->flags() & 0x20) != 0) {
			//TODO, not found anything that leads here yet
			return nullptr;
		}

		mono_class_t* ret = nullptr;

		const auto v10 = type_id & 0xFF000000;
		switch (v10) {
		case 0x1000000:
			//TODO, not found anything that leads here yet
			break;

		case 0x2000000: {
			ret = reinterpret_cast<mono_hash_table_t*>(this + 0x4C0)->lookup<mono_class_t>(reinterpret_cast<void*>(type_id));
			break;
		}

		case 0x1B000000:
			//TODO, not found anything that leads here yet
			break;

		default:
			break;
		}

		return ret;
	}
};

struct mono_assembly_t {
	OFFSET(mono_image(), mono_image_t*, 0x60)
};

namespace mono {
	inline mono_root_domain_t* get_root_domain() {
		return reinterpret_cast<mono_root_domain_t*>(driver.read<uintptr_t>(driver.dll(L"mono-2.0-bdwgc.dll") /* module handle of mono-2.0-bdwgc.dll */ + 0x499c78)); /* domain_free */
	}

	inline mono_assembly_t* domain_assembly_open(mono_root_domain_t* domain, const char* name) {
		auto domain_assemblies = domain->domain_assemblies();
		if (!domain_assemblies)
			return nullptr;

		auto data = uintptr_t();
		while (true) {
			data = domain_assemblies->data();
			if (!data)
				continue;

			const auto data_name = driver.read_string(data + 0x10);
			if (!strcmp(data_name.c_str(), name))
				break;

			domain_assemblies = reinterpret_cast<glist_t*>(domain_assemblies->next());
			if (!domain_assemblies)
				break;
		}

		return reinterpret_cast<mono_assembly_t*>(data);
	}

	inline mono_class_t* find_class(const char* assembly_name, const std::string& class_name) {
		const auto root_domain = get_root_domain();
		if (!root_domain)
			return nullptr;

		const auto domain_assembly = domain_assembly_open(root_domain, assembly_name);
		if (!domain_assembly)
			return nullptr;

		const auto img = domain_assembly->mono_image();
		if (!img)
			return nullptr;

		const auto type_def = img->get_table_info(2);
		if (!type_def)
			return nullptr;

		for (int i = 0; i < type_def->get_rows(); i++) {
			const auto ptr = static_cast<mono_hash_table_t*>(reinterpret_cast<void*>(img + 0x4C0))->lookup<mono_class_t>(reinterpret_cast<void*>(0x02000000 | i + 1));
			if (!ptr)
				continue;

			if (ptr->name() == class_name)
				return ptr;
		}

		return nullptr;
	}

	inline std::uintptr_t get_address_from_method(mono_method_t* method)
	{
		const auto root_domain = get_root_domain();
		if (!root_domain)
			return 0;

		const auto ptr = static_cast<mono_jit_hash_table_t*>(reinterpret_cast<void*>(root_domain + 0xF0))->lookup<std::uintptr_t>(reinterpret_cast<void*>(method));
		if (!ptr)
			return {};

		return driver.read<std::uintptr_t>(ptr + 0x10);
	}
}