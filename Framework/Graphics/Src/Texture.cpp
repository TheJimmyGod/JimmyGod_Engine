#include "Precompiled.h"
#include "Texture.h"

#include "D3DUtil.h"
#include <DirectXTK/Inc/WICTextureLoader.h>

using namespace JimmyGod::Graphics;

JimmyGod::Graphics::Texture::~Texture()
{
}

bool Texture::Initialize(const char* fileName)
{
	wchar_t wbuffer[1024];
	mbstowcs_s(nullptr, wbuffer, fileName, 1024);

	HRESULT hr = DirectX::CreateWICTextureFromFile(GetDevice(), GetContext(), wbuffer, nullptr, &mShaderResourceView);
	ASSERT(SUCCEEDED(hr), "Failed to load texture %ls.", fileName);

	ID3D11Resource* resource = nullptr;
	mShaderResourceView->GetResource(&resource);
	
	ID3D11Texture2D* texture = static_cast<ID3D11Texture2D*>(resource);
	D3D11_TEXTURE2D_DESC desc{};
	texture->GetDesc(&desc);

	mWidth = desc.Width;
	mHeight = desc.Height;
	return true;
}

bool Texture::Initialize(const std::filesystem::path& fileName)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(GetDevice(), GetContext(), fileName.c_str(), nullptr, &mShaderResourceView);
	ASSERT(SUCCEEDED(hr), "Failed to load texture %ls.", fileName);

	ID3D11Resource* resource = nullptr;
	mShaderResourceView->GetResource(&resource);

	ID3D11Texture2D* texture = static_cast<ID3D11Texture2D*>(resource);
	D3D11_TEXTURE2D_DESC desc{};
	texture->GetDesc(&desc);

	mWidth = desc.Width;
	mHeight = desc.Height;
	return true;
}

bool Texture::Initialize(const void * data, uint32_t width, uint32_t height)
{
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = data;
	initData.SysMemPitch = width * 4;

	ID3D11Device* device = GetDevice();

	ID3D11Texture2D* texture = nullptr;
	HRESULT hr = device->CreateTexture2D(&desc, &initData, &texture);
	if (FAILED(hr))
	{
		LOG("[Texture] Failed to create texture. HRESULT: 0x%x)", hr);
		return false;
	}

	hr = device->CreateShaderResourceView(texture, nullptr, &mShaderResourceView);
	if (FAILED(hr))
	{
		LOG("[Texture] Failed to create shader resource view. HRESULT: 0x%x)", hr);
		return false;
	}

	SafeRelease(texture);
	return true;
}

void Texture::Terminate()
{
	SafeRelease(mShaderResourceView);
}

void Texture::BindPS(uint32_t slot) const
{
	GetContext()->PSSetShaderResources(slot, 1, &mShaderResourceView);
}

void Texture::BindVS(uint32_t slot) const
{
	GetContext()->VSSetShaderResources(slot, 1, &mShaderResourceView);
}
