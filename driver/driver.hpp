#pragma once

#include <windows.h>

#include <string>

#include "../dependencies/xorstring/xorstring.hpp"
#include "../dependencies/lazy_importer/lazy_importer.hpp"

#include <memory>
#include <thread>

#include <iostream>

class driver_t
{
	struct data_t
	{
		enum
		{
			get_base,
			get_peb,
			copy_memory
		};

		std::uint64_t from_process_id, to_process_id;
		const void *from_address, *to_address;
		std::size_t size;
	};

	std::unique_ptr< std::remove_pointer_t< HANDLE >, decltype( &CloseHandle ) > bedaisy{ CreateFileA( "\\\\.\\BattlEye", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr ), &CloseHandle };

	const std::uint64_t from_process_id = GetCurrentProcessId( ), to_process_id;

public:
	explicit driver_t( ) : to_process_id{ 0 } { } bool initiate( const char* const window_name );

	template < class type = std::uintptr_t > type raw_read( const auto read_address )
	{
		type value;

		data_t data{ .from_process_id = to_process_id, .to_process_id = from_process_id, .from_address = reinterpret_cast< void* >( read_address ), .to_address = &value, .size = sizeof( type ) };
		if ( !DeviceIoControl( bedaisy.get( ), data_t::copy_memory, &data, 0xBE, nullptr, 0, nullptr, nullptr ) )
		{
			bedaisy = { CreateFileA( "\\\\.\\BattlEye", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr ), &CloseHandle };
			return raw_read< type >( read_address );
		}

		return value;
	}

	template < class value_t = std::uintptr_t > value_t __forceinline read( const auto read_address )
	{
		return raw_read< value_t >( read_address );
	}

	template < class buffer_t = std::uintptr_t >
	__forceinline buffer_t read( std::uintptr_t read_address, const std::initializer_list< std::uint16_t >&& offsets )
	{
		buffer_t buffer;

		for ( auto i = 0ull; i < offsets.size( ) - 1; ++i )
			read_address = raw_read<std::uintptr_t>( read_address + *( offsets.begin( ) + i ) );

		return read< buffer_t >( read_address + *( offsets.end( ) - 1 ) );
	}

	template < class value_t = std::uintptr_t > void read_arr( const auto read_address, value_t* buffer, std::size_t size )
	{
		data_t data{ .from_process_id = to_process_id, .to_process_id = from_process_id, .from_address = reinterpret_cast< void* >( read_address ), .to_address = buffer, .size = sizeof( value_t ) * size };

		if ( !DeviceIoControl( bedaisy.get( ), data_t::copy_memory, &data, 0xBE, nullptr, 0, nullptr, nullptr ) )
		{
			bedaisy = { CreateFileA( "\\\\.\\BattlEye", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr ), &CloseHandle };
		}
	}

	template < class type >
	void write( const auto write_address, const type& value )
	{
		data_t data{ .from_process_id = from_process_id, .to_process_id = to_process_id, .from_address = &value, .to_address = reinterpret_cast< void* >( write_address ), .size = sizeof( type ) };
		if ( !DeviceIoControl( bedaisy.get( ), data_t::copy_memory, &data, 0xBE, nullptr, 0, nullptr, nullptr ) )
		{
			bedaisy = { CreateFileA( "\\\\.\\BattlEye", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr ), &CloseHandle };
		}
	}

	template < class value_t = std::uintptr_t > void write_arr(const std::uintptr_t address, value_t* buffer, const std::size_t size) const
	{
		data_t data{ .from_process_id = from_process_id, .to_process_id = to_process_id, .from_address = &buffer, .to_address = reinterpret_cast< void* >( address ), .size = sizeof( value_t ) * size };
		if ( !DeviceIoControl( bedaisy.get( ), data_t::copy_memory, &data, 0xBE, nullptr, 0, nullptr, nullptr ) )
		{
			bedaisy = { CreateFileA( "\\\\.\\BattlEye", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr ), &CloseHandle };
		}
	}

	std::uintptr_t base( )
	{
		static std::uintptr_t base;
		if ( !base )
		{
			data_t data{ .from_process_id = from_process_id, .to_process_id = to_process_id, .from_address = &base };
			DeviceIoControl( bedaisy.get( ), data_t::get_base, &data, 0xBE, nullptr, 0, nullptr, nullptr );
		}

		return base;
	}

	std::uintptr_t peb( )
	{
		static std::uintptr_t peb;
		if ( !peb )
		{
			data_t data{ .from_process_id = from_process_id, .to_process_id = to_process_id, .from_address = &peb };
			DeviceIoControl( bedaisy.get( ), data_t::get_peb, &data, 0xBE, nullptr, 0, nullptr, nullptr );
		}

		return peb;
	}

	std::uintptr_t dll( const std::wstring& dll_name )
	{
		{
			auto v5 = read( peb( ) + 0x18 );

			if ( v5 )
			{
				auto v6 = read( v5 + 16 );
				if ( v6 )
				{
					while ( read( v6 + 0x30 ) )
					{
						auto length = read<USHORT>( v6 + 0x58 );

						auto start = read( v6 + 0x60 );

						std::wstring name{};

						name.reserve( length / 2 );

						for ( auto i = 0u; i < length / 2; ++i )
						{
							name.push_back( read< WCHAR>( start + i * 2 ) );
						}

						if ( name == dll_name )
							return read( v6 + 0x30 );

						v6 = read( v6 );
						if ( !v6 )
							return 0;
					}
				}
			}
		}
	}

	std::string read_string( std::uintptr_t address, const bool inner = true ) const; std::string read_unicode_string( std::uintptr_t address, const int size ) const;

} extern driver;