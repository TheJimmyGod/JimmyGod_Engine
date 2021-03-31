#pragma once

namespace JimmyGod::Graphics {

class Texture
{
public:
	Texture() = default;
	~Texture();
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	bool Initialize(const std::filesystem::path& fileName);
	void Terminate();

	void BindVS(uint32_t slot = 0) const;
	void BindPS(uint32_t slot = 0) const;

	uint32_t GetWidth() const { return mWidth; }
	uint32_t GetHeight() const { return mHeight; }

	void* GetRawData() { return mShaderResourceView; }

private:
	friend class SpriteRenderer;

	ID3D11ShaderResourceView* mShaderResourceView = nullptr;
	uint32_t mWidth;
	uint32_t mHeight;
};

}