#pragma once
#include "Input/Inc/InputTypes.h"
#include "Texture.h"

namespace JimmyGod::Graphics
{
	using TextureId = size_t;

	class TextureManager
	{
	public:
		static void StaticInitialize(const std::filesystem::path& root);
		static void StaticTerminate();
		static TextureManager* Get();
	public:
		TextureManager();
		~TextureManager();

		TextureManager(const TextureManager&) = delete;
		TextureManager& operator=(const TextureManager&) = delete;

		void SetRootPath(const std::filesystem::path& path);

		TextureId Load(const std::filesystem::path& fileName, bool isRootPath);
		void Clear();

		void BindVS(TextureId id, uint32_t slot = 0);
		void BindPS(TextureId id, uint32_t slot = 0);

		Texture* GetTexture(TextureId id);
	private:
		std::filesystem::path mRoot;
		std::unordered_map<std::size_t, Texture*> mInventory;
	};
}