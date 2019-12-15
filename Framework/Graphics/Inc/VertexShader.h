#pragma once

namespace JimmyGod::Graphics
{
	class VertexShader
	{
	public:
		void Initialize(const std::filesystem::path& filePath, uint32_t vertexFormat);
		void Terminate();

		void Bind() const;
	private:
		ID3D11VertexShader* mVertexShader = nullptr;
		ID3D11InputLayout* mInputLayout = nullptr;
	};
} // namespace JimmyGod::Graphics