#include "Precompiled.h"
#include "MeshBuffer.h"
#include "GraphicTypes.h"
#include "D3DUtil.h"

using namespace JimmyGod;
using namespace JimmyGod::Graphics;

void MeshBuffer::Initialize(Vertex* vertices, int vertexCount, uint32_t* indices, UINT indexCount)
{
	auto device = GetDevice();

	// Create vertex buffer
	bufferDesc.ByteWidth = vertexCount * sizeof(Vertex);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0; // Use CPU
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertices;

	HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer");

	// Create index buffer
	bufferDesc.ByteWidth = indexCount * sizeof(uint32_t);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	initData.pSysMem = indices;
	hr = device->CreateBuffer(&bufferDesc, &initData, &mIndexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create index buffer");
}

void MeshBuffer::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mIndexBuffer);
}

void MeshBuffer::Draw()
{
	mIndexBuffer->GetDesc(&bufferDesc);
	auto count = bufferDesc.ByteWidth / sizeof(uint32_t);
	auto context = GetContext();
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(count, 0, 0);
}
