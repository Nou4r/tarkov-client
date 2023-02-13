#include "driver.hpp"
#include "../integrity/integrity.hpp"

#include <tlhelp32.h>
#include <iostream>

driver_t driver;

bool driver_t::initiate( const char* const window_name )
{
	return lf( GetWindowThreadProcessId )( lf( FindWindowA )( nullptr, window_name ), ( DWORD* )&to_process_id );
}

std::string driver_t::read_string(std::uintptr_t address, const bool inner ) const
{
	if ( inner )
		address = driver.read( address );

	std::string string;

	do
		string.push_back( driver.read< char >( address++ ) );
	while ( driver.read< char >( address ) != '\0' );

	return string;
}

std::string driver_t::read_unicode_string( std::uintptr_t address, const int size ) const
{
	char16_t temporary[128] = { '\0' };

	driver.read_arr<char16_t>( address, temporary, size * sizeof( std::uint16_t ) );
	std::string buffer = 
		integrity::utilities::to_utf8( temporary );
	
	if ( !buffer.empty( ) )
		return buffer;
	else
		return "";
}