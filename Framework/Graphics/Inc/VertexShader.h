#pragma once

namespace JimmyGod::Graphics
{
	class VertexShader
	{
	public:
		void Initialize();
		void Terminate();

		void Bind();
	private:
		ID3D11VertexShader* mVertexShader = nullptr;
		ID3D11InputLayout* mInputLayout = nullptr;
	};
} // namespace JimmyGod::Graphics