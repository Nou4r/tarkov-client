#pragma once

#include "../../renderer.hpp"

#include "../../../dependencies/stb/stb_image.h"

namespace image::d3dx
{
	extern ID3D11ShaderResourceView* intellisense_icon; 

	extern ID3D11ShaderResourceView* player_image;
	extern ID3D11ShaderResourceView* skeleton_image;

	bool LoadTextureFromMemoryDX( ID3D11Device* device, const void* pdata, unsigned size, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height );
	bool LoadTextureFromMemoryDXL( ID3D11Device* device, const void* pdata, unsigned size, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height );

	bool LoadTextureFromMemory( ID3D11Device* device, const void* pdata, unsigned size, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height );
}