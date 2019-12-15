#pragma once
namespace JimmyGod::Graphics
{
	class PixelShader
	{
	public:
		void Initialize(const std::filesystem::path& filePath);
		void Terminate();

		void Bind() const;

	private:
		ID3D11PixelShader* mPixelShader = nullptr;
	};
} //namespace JimmyGod::Graphics