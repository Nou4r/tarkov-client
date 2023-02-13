#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

#include <wrl.h>
#include <wrl/client.h>

#include <d3d11.h>

#include <dcomp.h>

#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>

#include <windows.h>

#include <memory>
#include <atomic>

#include <string_view>

#include <functional>

#include <dwmapi.h>
#pragma comment ( lib, "dwmapi" )

#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_impl_dx11.h"
#include "../dependencies/imgui/imgui_impl_win32.h"
#include "../dependencies/imgui/imgui_internal.h"

#include "../dependencies/font/hashes.h"
#include "../dependencies/font/sffont.hpp"
#include "../dependencies/font/poppins.hpp"
#include "../dependencies/font/faprolight.hpp"
#include "../dependencies/font/drukwidebold.hpp"

#include "../dependencies/xorstring/xorstring.hpp"

#pragma comment(  lib, "dxgi"  )
#pragma comment(  lib, "dcomp"  )

#pragma comment( lib, "d3d11.lib" )

#pragma comment( lib, "comctl32.lib" )

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace renderer::structures
{
	struct renderer_t
	{
		HWND overlay_window, game_window;

		RECT overlay_rect{ 0 }; RECT game_rect{ 0 };

		const float alpha_channel[4] = { 0.f, 0.f, 0.f, 0.f };

		bool is_menu_open = false; bool is_watermark_shown = true;

		void release_render_target();

		void create_render_target();

		void initialize_imgui();

		void initialize_device();

		void render();

		renderer_t(HWND game_window);
	};
}

namespace renderer::values
{
	extern std::unique_ptr< renderer::structures::renderer_t > renderer;

	extern std::atomic< bool > insert_hit;
}

namespace renderer
{
	void start(std::string_view window_name);

	LRESULT __stdcall wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
}

namespace renderer::imgui::elements
{
	enum button_flags
	{
		BUTTON_PURPLE,

		BUTTON_GREEN,
		BUTTON_BLUE,
		BUTTON_RED,

		BUTTON_GRAY
	};

	namespace fonts
	{
		inline ImFont* pro_font_windows;
		inline ImFont* bender_font_reg;

		inline ImFont* druk_wide_bold;
		inline ImFont* px18;
		inline ImFont* px24;
	}

	namespace static_vars
	{
		inline std::string current_tab = xs("LEGIT");
	}

	namespace render
	{
		extern void set_styles(void*);
	}

	namespace functions
	{
		extern void tab(const char* label, int& tab, int index, std::function<void()> cb = nullptr);

		extern void begin_child(const char* str_id, const ImVec2 size_arg, bool border = false, ImGuiWindowFlags extra_flags = NULL);
		extern void end_child();

		extern void checkbox(const char* label, const char* desc, bool& v);

		extern void separator();
		extern void separator_wide();

		extern bool hotkey(const char* label, int* k);
		extern bool hotkey_stretched(const char* label, int* k);

		extern void information(const char* label, const char* desc, float p_data, const char* format);
		extern void information(const char* label, const char* desc, int p_data, const char* format);
		extern void information(const char* label, const char* desc, const char* p_data, const char* icon);
		extern void information(const char* label, const char* desc);

		extern bool slider_float(const char* label, float* v, float v_min, float v_max, const char* format, float power);
		extern bool slider_int(const char* label, int* v, int v_min, int v_max, const char* format);
		extern bool slider_float_stretched(const char* label, float* v, float v_min, float v_max, const char* format, float power);
		extern bool slider_int_stretched(const char* label, int* v, int v_min, int v_max, const char* format);

		extern bool begin_combo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);
		extern void end_combo();

		extern bool combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
		extern bool combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);
		extern bool combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1);

		extern void color_picker(const char* identifier, float* col);
		extern bool button(const char* label, ImVec2 size_arg, renderer::imgui::elements::button_flags flag);
		extern bool config_button(const char* label, const char* data, const char* author);

		extern bool tab(const char* label, const char* icon, int& tab, int index);
		extern bool subtab_static(const char* label, const char* icon, int& tab, int index);
	}
}