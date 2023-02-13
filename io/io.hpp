#pragma once

#include <iostream>
#include <Windows.h>

namespace io
{
	__forceinline void open_console( const char* const console_name )
	{
		FILE* input_output_stream;

		AllocConsole( );

		freopen_s( &input_output_stream, "CONIN$", "r", stdin );
		freopen_s( &input_output_stream, "CONOUT$", "w", stdout );
		freopen_s( &input_output_stream, "CONOUT$", "w", stderr );

		SetConsoleTitleA(console_name);
	}
}