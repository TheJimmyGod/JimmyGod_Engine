#include "Precompiled.h"
#include "AlphaBlending.h"

#include "D3DUtil.h"

using namespace JimmyGod;
using namespace JimmyGod::Graphics;

void AlphaBlending::Initialize()
{
	D3D11_BLEND_DESC omDesc;
	ZeroMemory(&omDesc, sizeof(D3D11_BLEND_DESC));
	omDesc.RenderTarget[0].BlendEnable = true;

	omDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	omDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	omDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	omDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	omDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	auto device = GetDevice();
	auto hr = device->CreateBlendState(&omDesc, &mBlendState);

	ASSERT(SUCCEEDED(hr), "Failed to create Alpha Blending.");
}

void AlphaBlending::Terminate()
{
	SafeRelease(mBlendState);
}

void AlphaBlending::Bind() const
{
	GetContext()->OMSetBlendState(mBlendState, 0, 0xffffffff);
}

void AlphaBlending::UnBind() const
{
	GetContext()->OMSetBlendState(nullptr, 0, 0xffffffff);
}
