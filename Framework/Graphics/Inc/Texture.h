#pragma once

namespace JimmyGod::Graphics {

class Texture
{
public:
	Texture() = default;
	~Texture();
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	void Initialize(const std::filesystem::path& fileName);
	void Terminate();

	void BindVS(uint32_t slot = 0) const;
	void BindPS(uint32_t slot = 0) const;
private:
	ID3D11ShaderResourceView* mShaderResourceView = nullptr;
};

} // namespace PCEngine::Graphics