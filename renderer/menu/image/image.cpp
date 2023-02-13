#include "image.hpp"

ID3D11ShaderResourceView* image::d3dx::intellisense_icon = nullptr;

ID3D11ShaderResourceView* image::d3dx::player_image = nullptr;
ID3D11ShaderResourceView* image::d3dx::skeleton_image = nullptr;

typedef struct D3DX11_IMAGE_LOAD_INFO {
	UINT              Width;
	UINT              Height;
	UINT              Depth;
	UINT              FirstMipLevel;
	UINT              MipLevels;
	D3D11_USAGE       Usage;
	UINT              BindFlags;
	UINT              CpuAccessFlags;
	UINT              MiscFlags;
	DXGI_FORMAT       Format;
	UINT              Filter;
	UINT              MipFilter;
	void* SrcInfo;
} D3DX11_IMAGE_LOAD_INFO, * LPD3DX11_IMAGE_LOAD_INFO;

HRESULT D3DX11CreateShaderResourceViewFromMemory(
	_In_  ID3D11Device* pDevice,
	_In_  LPCVOID                  pSrcData,
	_In_  SIZE_T                   SrcDataSize,
	_In_  D3DX11_IMAGE_LOAD_INFO* pLoadInfo,
	_In_  void* pPump,
	_Out_ ID3D11ShaderResourceView** ppShaderResourceView,
	_Out_ HRESULT* pHResult
)
{
	static decltype( D3DX11CreateShaderResourceViewFromMemory )* fn = nullptr;
	if ( !fn )	
		fn = reinterpret_cast< decltype( fn ) >( GetProcAddress( LoadLibraryExA( "d3dx11_42.dll", 0, 0 ),( "D3DX11CreateShaderResourceViewFromMemory" ) ) );	

	return fn( pDevice, pSrcData, SrcDataSize, pLoadInfo, pPump, ppShaderResourceView, pHResult );
}

HRESULT D3DX11CreateShaderResourceViewFromFile(
	_In_  ID3D11Device* pDevice,
	_In_  LPCTSTR                  pSrcFile,
	_In_  D3DX11_IMAGE_LOAD_INFO* pLoadInfo,
	_In_  void* pPump,
	_Out_ ID3D11ShaderResourceView** ppShaderResourceView,
	_Out_ HRESULT* pHResult
)
{
	static decltype( D3DX11CreateShaderResourceViewFromFile )* fn = nullptr;
	if ( !fn )
	{
		fn = reinterpret_cast< decltype( fn ) >( GetProcAddress( LoadLibraryExA( "d3dx11_42.dll", 0, 0 ),( "D3DX11CreateShaderResourceViewFromFileW" ) ) );
	}

	return fn( pDevice, pSrcFile, pLoadInfo, pPump, ppShaderResourceView, pHResult );
}

bool image::d3dx::LoadTextureFromMemoryDX( ID3D11Device* device, const void* pdata, unsigned size, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height )
{
	D3DX11_IMAGE_LOAD_INFO imageinfo = {};
	HRESULT res;
	D3DX11CreateShaderResourceViewFromMemory( device, pdata, size, nullptr, nullptr, out_srv, nullptr );

	if ( out_width && out_height )
	{
		*out_width = imageinfo.Width;
		*out_height = imageinfo.Height;
	}

	return true;
}

bool image::d3dx::LoadTextureFromMemoryDXL( ID3D11Device* device, const void* pdata, unsigned size, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height )
{
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load_from_memory( ( const stbi_uc* )pdata, ( int )size, &image_width, &image_height, NULL, 4 );
	if ( image_data == NULL )
		return false;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof( desc ) );
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = desc.Height * 4;
	device->CreateTexture2D( &desc, &subResource, &pTexture );

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory( &srvDesc, sizeof( srvDesc ) );
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	device->CreateShaderResourceView( pTexture, &srvDesc, out_srv );
	pTexture->Release( );

	if ( out_width && out_height )
	{
		*out_width = image_width;
		*out_height = image_height;
	}
	free( image_data );

	return true;
}

bool image::d3dx::LoadTextureFromMemory( ID3D11Device* device, const void* pdata, unsigned size, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height )
{
	return LoadTextureFromMemoryDX( device, pdata, size, out_srv, out_width, out_height );
}