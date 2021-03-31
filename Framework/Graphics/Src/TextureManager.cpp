#include "Precompiled.h"
#include "TextureManager.h"

using namespace JimmyGod::Graphics;

namespace { std::unique_ptr<TextureManager> sTextureManager = nullptr; }

void TextureManager::StaticInitialize(const std::filesystem::path& root)
{
	ASSERT(sTextureManager == nullptr, "[TextureManager] Manager already initialized!");
	sTextureManager = std::make_unique<TextureManager>();
	sTextureManager->SetRootPath(root);
}

void TextureManager::StaticTerminate()
{
	if (sTextureManager != nullptr)
	{
		sTextureManager->Clear();
		sTextureManager.reset();
	}
}

TextureManager * TextureManager::Get()
{
	return sTextureManager.get();
}

TextureManager::TextureManager() {}
TextureManager::~TextureManager()
{
	ASSERT(mInventory.empty(), "[TextureManager] Clear() must be called to clean up.");
}

void TextureManager::SetRootPath(const std::filesystem::path& path)
{
	mRoot = path;
}

TextureId TextureManager::Load(const std::filesystem::path& fileName, bool isRootPath)
{
	std::filesystem::path fullName = mRoot;

	fullName = (isRootPath) ? fullName / fileName : fileName;

	std::hash<std::string> hasher;
	TextureId hash = hasher(fullName.u8string());

	auto result = mInventory.insert({ hash, nullptr });
	if (result.second)
	{
		Texture* texture = new Texture();
		if (texture->Initialize(fullName.c_str()))
		{
			result.first->second = texture;
		}
		else
		{
			delete texture;
			texture = nullptr;
			mInventory.erase(result.first);
			hash = 0;
		}
	}

	return hash;
}

void TextureManager::Clear()
{
	for (auto& item : mInventory)
	{
		if (item.second)
		{
			item.second->Terminate();
			delete item.second;
			item.second = nullptr;
		}
	}
	mInventory.clear();
}

void TextureManager::BindVS(TextureId id, uint32_t slot)
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		iter->second->BindVS(slot);
	}
}

void TextureManager::BindPS(TextureId id, uint32_t slot)
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		iter->second->BindPS(slot);
	}
}

Texture* TextureManager::GetTexture(TextureId id)
{
	auto iter = mInventory.find(id);
	return iter != mInventory.end() ? iter->second : nullptr;
}