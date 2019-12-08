#include "Precompiled.h"
#include "D3DUtil.h"

#include "GraphicsSystem.h"

ID3D11Device* JimmyGod::Graphics::GetDevice()
{
	return GraphicsSystem::Get()->mD3DDevice;
}

ID3D11DeviceContext* JimmyGod::Graphics::GetContext()
{
	return GraphicsSystem::Get()->mImmediateContext;
}
