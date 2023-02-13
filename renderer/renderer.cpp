#include "renderer.hpp"

#include <thread>
#include <winuser.h>

#include "menu/menu.hpp"
#include "menu/image/image.hpp"

#include "../settings/settings.hpp"

#include "../dependencies/imgui/textures/icons/intellisense.hpp"
#include "../dependencies/imgui/textures/skeleton.hpp"
#include "../dependencies/imgui/textures/player.hpp"

#include "../dependencies/xorstring/xorstring.hpp"
#include "../dependencies/lazy_importer/lazy_importer.hpp"

std::unique_ptr< renderer::structures::renderer_t > renderer::values::renderer;
std::atomic< bool > renderer::values::insert_hit = false;

Microsoft::WRL::ComPtr< ID3D11Device > global_device;
Microsoft::WRL::ComPtr< ID3D11DeviceContext > global_device_context;
Microsoft::WRL::ComPtr< IDXGISwapChain1 > global_swap_chain;
Microsoft::WRL::ComPtr< ID3D11RenderTargetView > render_target_view;

Microsoft::WRL::ComPtr< IDXGIDevice > global_dxgi_device;
Microsoft::WRL::ComPtr< IDXGIFactory2 > global_dx_factory;

Microsoft::WRL::ComPtr< IDCompositionDevice > global_dcomp_device;
Microsoft::WRL::ComPtr< IDCompositionVisual > global_dcomp_visual;

void renderer::structures::renderer_t::initialize_imgui( )
{
	ImGui::CreateContext( );

	renderer::imgui::elements::render::set_styles( NULL );

	ImGui_ImplWin32_Init( overlay_window );
	ImGui_ImplDX11_Init( global_device.Get( ), global_device_context.Get( ) );
}

void renderer::structures::renderer_t::create_render_target( )
{
	Microsoft::WRL::ComPtr< ID3D11Texture2D > texture_buffer;

	if ( FAILED( global_swap_chain->GetBuffer( 0u, IID_PPV_ARGS( texture_buffer.GetAddressOf( ) ) ) ) )
	std::printf( xs( "FAILED TBUFFER" ) );

	if ( FAILED( global_device->CreateRenderTargetView( texture_buffer.Get( ), NULL, &render_target_view ) ) )
	std::printf( xs( "FAILED TVIEW" ) );
}

void renderer::structures::renderer_t::release_render_target( )
{
	render_target_view.Reset( );
}

void renderer::structures::renderer_t::initialize_device( )
{
    DXGI_SWAP_CHAIN_DESC1 DXGI_DESC1;

    RtlZeroMemory( &DXGI_DESC1, sizeof( DXGI_SWAP_CHAIN_DESC1 ) );
	DXGI_DESC1.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	DXGI_DESC1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	DXGI_DESC1.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
	DXGI_DESC1.Scaling = DXGI_SCALING_STRETCH;
	DXGI_DESC1.Width = 1;
	DXGI_DESC1.Height = 1;
	DXGI_DESC1.SampleDesc.Count = 1;
	DXGI_DESC1.SampleDesc.Quality = 0u;
	DXGI_DESC1.BufferCount = 2;
	DXGI_DESC1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

    if ( FAILED( 
		global_dx_factory->CreateSwapChainForComposition(
			global_dxgi_device.Get( ), &DXGI_DESC1,
		nullptr, global_swap_chain.GetAddressOf( ) ) ) )
        std::printf( xs( "FAILED DEVICE" ) );

    if ( FAILED( DCompositionCreateDevice(global_dxgi_device.Get( ),
		IID_PPV_ARGS( 
			global_dcomp_device.GetAddressOf( ) ) ) ) )
        std::printf( xs( "FAILED COMPDEVICE" ) );

	IDCompositionTarget* target;
	auto h_result = global_dcomp_device->CreateTargetForHwnd( overlay_window, true, &target );

	h_result = global_dcomp_device->CreateVisual( global_dcomp_visual.GetAddressOf( ) );

	h_result = global_dcomp_visual->SetContent( global_swap_chain.Get( ) );
	h_result = target->SetRoot( global_dcomp_visual.Get( ) ); h_result = global_dcomp_device->Commit( );
}

void renderer::structures::renderer_t::render( )
{
	MSG msg{ };
	RtlZeroMemory( &msg, sizeof( MSG ) );

	while ( msg.message != WM_QUIT )
	{
		if ( PeekMessage( &msg, NULL, 0u, 0u, PM_REMOVE ) != WM_NULL )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
			continue;
		}

		{
			bool is_overlay_visible = lf( IsWindowVisible )( overlay_window );

			lf( GetWindowRect )( overlay_window, &overlay_rect ); lf( GetWindowRect )( game_window, &game_rect );
			
			bool is_game_foreground = game_window == GetForegroundWindow( );

			if ( !is_game_foreground )
			{
				if ( IsWindowVisible( overlay_window ) )
					ShowWindow( overlay_window, SW_HIDE );

				continue;
			}

			if ( !is_overlay_visible ||
				( overlay_rect.right != game_rect.right && overlay_rect.left != game_rect.left &&
					overlay_rect.bottom != game_rect.bottom && overlay_rect.top != game_rect.top ) )
			{
				lf( SetWindowPos )( overlay_window, HWND_TOPMOST, game_rect.left, game_rect.top, game_rect.right - game_rect.left, game_rect.bottom - game_rect.top, SWP_SHOWWINDOW );

				release_render_target( );

				global_swap_chain->ResizeBuffers( 0u, game_rect.right - game_rect.left, game_rect.bottom - game_rect.top, DXGI_FORMAT_UNKNOWN, 0u );

				create_render_target( );
			}

			bool overlay_context = ( GetForegroundWindow( ) == overlay_window || GetForegroundWindow( ) == game_window );

			auto ex_style = GetWindowLong( overlay_window, GWL_EXSTYLE );

			if ( renderer::values::insert_hit )
			{
				renderer::values::insert_hit = false;

				if ( overlay_context )
				{
					is_menu_open = !is_menu_open;

					if ( is_menu_open )
					{
						lf( SetForegroundWindow )( overlay_window );

						SetWindowLong( overlay_window, GWL_EXSTYLE, ex_style & ~WS_EX_TRANSPARENT );
					}
					else
					{
						lf( SetForegroundWindow )( game_window );

						SetWindowLong( overlay_window, GWL_EXSTYLE, ex_style | WS_EX_TRANSPARENT );
					}
				}
			}
		}
		
		global_device_context->ClearRenderTargetView( render_target_view.Get( ), renderer::values::renderer->alpha_channel );

		ImGui_ImplDX11_NewFrame( );
		
		ImGui_ImplWin32_NewFrame( );
		
		ImGui::NewFrame( );
		{
			renderer::menu::draw( );
		} ImGui::Render( );

		global_device_context->OMSetRenderTargets( 1u, render_target_view.GetAddressOf( ), NULL );
		
		ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );
		
		global_swap_chain->Present( 0u, 0u );
	}
}

renderer::structures::renderer_t::renderer_t( HWND game_window )
	: game_window{ game_window }
{
	WNDCLASS window_class = { };
	window_class.lpszClassName = "Windows.UI.Core.CoreWindow";
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = wndproc;

	if ( FAILED( RegisterClass( &window_class ) ) )
		std::printf( xs( "FAILED WINCLASS" ) );

	overlay_window = lf( CreateWindowExA )( WS_EX_WINDOWEDGE | WS_EX_NOACTIVATE | WS_EX_NOREDIRECTIONBITMAP | WS_EX_LAYERED | WS_EX_TRANSPARENT, window_class.lpszClassName, "NZXT CAM", WS_POPUP | WS_CLIPSIBLINGS, 0u, 0u, 940, 885, nullptr, nullptr, nullptr, nullptr );
	
	if ( !overlay_window )
		std::printf( xs( "FAILED WINDOW" ) );

	if ( FAILED( 
		D3D11CreateDevice( nullptr, 
			D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0u,
		D3D11_SDK_VERSION, global_device.GetAddressOf( ), nullptr, global_device_context.GetAddressOf( ) ) ) )
		std::printf( xs( "FAILED D3DEVICE" ) );

	if ( FAILED( global_device.As( &global_dxgi_device ) ) )
		std::printf( xs( "FAILED DXGIDEVICE" ) );

	if ( FAILED( CreateDXGIFactory2( 0, IID_PPV_ARGS( global_dx_factory.GetAddressOf( ) ) ) ) )
		std::printf( xs( "FAILED DXGIFAC" ) );
}

void renderer::imgui::elements::render::set_styles(void*)
{
	ImGui::GetStyle( ).WindowPadding = { 0u,0u };
	ImGui::GetStyle( ).WindowBorderSize = 0u;
	ImGui::GetStyle( ).PopupBorderSize = 0u;
	ImGui::GetStyle( ).PopupRounding = 5;
	ImGui::GetStyle( ).WindowRounding = 16;
	ImGui::GetStyle( ).ScrollbarSize = 4;

	ImGui::GetStyle( ).Colors[ImGuiCol_WindowBg] = ImColor{ 18, 18, 18 };
	ImGui::GetStyle( ).Colors[ImGuiCol_PopupBg] = ImColor{ 33, 33, 33 };
	ImGui::GetStyle( ).Colors[ImGuiCol_Text] = ImColor{ 119, 119, 119 };
	ImGui::GetStyle( ).Colors[ImGuiCol_ScrollbarBg] = ImColor{ 119, 119, 119, 0u };

	ImGui::GetStyle( ).Colors[ImGuiCol_Header] = ImColor{ 35, 42, 51 };
	ImGui::GetStyle( ).Colors[ImGuiCol_HeaderActive] = ImColor{ 35, 42, 51 };
	ImGui::GetStyle( ).Colors[ImGuiCol_HeaderHovered] = ImColor{ 35, 42, 51 };

	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0u };

	static const ImWchar ranges[] = {
	0x0020, 0x00FF,0x2000, 0x206F,0x3000, 0x30FF,0x31F0, 0x31FF, 0xFF00,
	0xFFEF,0x4e00, 0x9FAF,0x0400, 0x052F,0x2DE0, 0x2DFF,0xA640, 0xA69F, 0
	};

	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;

	ImGui::GetIO( ).Fonts->AddFontFromMemoryCompressedTTF( poppins_compressed_data, poppins_compressed_size, 14, NULL, ranges );
	ImGui::GetIO( ).Fonts->AddFontFromMemoryTTF( faprolight, sizeof faprolight, 18, &icons_config, icon_ranges );

	ImGui::GetIO( ).IniFilename = NULL;

	renderer::imgui::elements::fonts::pro_font_windows = ImGui::GetIO( ).Fonts->AddFontFromMemoryTTF( visitor_font, sizeof visitor_font, 18, NULL, ranges );
	renderer::imgui::elements::fonts::bender_font_reg = ImGui::GetIO().Fonts->AddFontFromMemoryTTF( bender_font_reg, sizeof bender_font_reg, 18, NULL, ranges );

	renderer::imgui::elements::fonts::druk_wide_bold = ImGui::GetIO( ).Fonts->AddFontFromMemoryCompressedTTF( druk_wide_bold_compressed_data, druk_wide_bold_compressed_size, 30u, NULL, ranges );

	renderer::imgui::elements::fonts::px18 = ImGui::GetIO( ).Fonts->AddFontFromMemoryCompressedTTF( poppins_compressed_data, poppins_compressed_size, 18, NULL, ranges );
	renderer::imgui::elements::fonts::px24 = ImGui::GetIO( ).Fonts->AddFontFromMemoryCompressedTTF( poppins_compressed_data, poppins_compressed_size, 24, NULL, ranges );

	image::d3dx::LoadTextureFromMemory( global_device.Get( ), player_texture, sizeof player_texture, &image::d3dx::player_image, nullptr, nullptr );
	image::d3dx::LoadTextureFromMemory( global_device.Get( ), skeleton_texture, sizeof skeleton_texture, &image::d3dx::skeleton_image, nullptr, nullptr );

	image::d3dx::LoadTextureFromMemory( global_device.Get( ), intellisense_icon, sizeof intellisense_icon, &image::d3dx::intellisense_icon, nullptr, nullptr );
}

void renderer::start( std::string_view window_name )
{
	const auto game_window = FindWindowA( nullptr, window_name.data( ) );

	renderer::values::renderer = std::make_unique< renderer::structures::renderer_t >( game_window );

	values::renderer->initialize_device( );

	values::renderer->initialize_imgui( );

	std::thread( [=]( ) {
		RegisterHotKey( 0u, 1, 0u, VK_INSERT );

		MSG hmsg = { 0u };
		while ( true )
		{
			if ( GetMessage( &hmsg, nullptr, 0u, 0u ) && hmsg.message == WM_HOTKEY && !renderer::values::insert_hit )
				renderer::values::insert_hit = true;

			std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
		}
		} ).detach( );

	values::renderer->render( );
}

LRESULT __stdcall renderer::wndproc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	switch ( msg )
	{
	case WM_SIZE:
		if ( global_swap_chain )
		{
			values::renderer->release_render_target( );

			global_swap_chain->ResizeBuffers( 0u, (UINT)LOWORD( lparam ), (UINT)HIWORD( lparam ), DXGI_FORMAT_UNKNOWN, 0u );

			values::renderer->create_render_target( );
		}
		return 0u;
	case WM_SYSCOMMAND:
		if ( ( wparam & 0xFFF0 ) == SC_KEYMENU )
			return 0u;

		break;
	case WM_DESTROY:
		::PostQuitMessage( 0u );

		return 0u;
	default:
		if ( ImGui_ImplWin32_WndProcHandler( hwnd, msg, wparam, lparam ) )
			return S_OK;
		break;
	}

	return ::DefWindowProc( hwnd, msg, wparam, lparam );
}