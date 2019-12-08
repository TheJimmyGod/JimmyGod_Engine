#pragma once
#include "GraphicTypes.h"
namespace JimmyGod::Graphics
{
class MeshBuffer
{
public:
	void Initialize(Vertex* vertices, int vertexCount, uint32_t* indices, UINT indexCount);
	void Terminate();
	void Draw(UINT size);
private:
	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11Buffer* mIndexBuffer = nullptr;
};
}