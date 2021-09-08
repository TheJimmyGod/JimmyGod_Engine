#pragma once

namespace JimmyGod::Graphics {

class Texture
{
public:
	Texture() = default;
	~Texture();
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	bool Initialize(const char* fileName);
	bool Initialize(const std::filesystem::path& fileName);
	bool Initialize(const void* data, uint32_t width, uint32_t height);
	void Terminate();

	void BindVS(uint32_t slot = 0) const;
	void BindPS(uint32_t slot = 0) const;

	bool IsInitialized() const { return mInitialized; }

	uint32_t GetWidth() const { return mWidth; }
	uint32_t GetHeight() const { return mHeight; }

	void* GetRawData() { return mShaderResourceView; }
	ID3D11ShaderResourceView* GetShaderResourceView() { return mShaderResourceView; }

private:
	friend class SpriteRenderer;

	ID3D11ShaderResourceView* mShaderResourceView;
	uint32_t mWidth;
	uint32_t mHeight;

	bool mInitialized = false;
};

}