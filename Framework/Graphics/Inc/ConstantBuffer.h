#pragma once

namespace JimmyGod::Graphics
{
	class ConstantBuffer
	{
	public:
		void Initialize(uint32_t size);
		void Terminate();
		void Bind();
		void Set(void* data);
	private:
		ID3D11Buffer* mConstantBuffer = nullptr;
		float mRotation = 0.0f;
	};
}