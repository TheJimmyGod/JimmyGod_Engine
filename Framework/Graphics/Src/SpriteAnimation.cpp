#include "Precompiled.h"
#include "SpriteAnimation.h"

#include "SpriteRenderManager.h"
#include "TextureManager.h"
#include "TextureType.h"

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

void SpriteAnimation::Load(JimmyGod::Graphics::SpriteAnimationInfo info)
{
	mInfo = std::move(info); // Change ownership
	mTextureID = TextureManager::Get()->Load(mInfo.fileName.c_str());
	mTimer = 0.0f;
}

void SpriteAnimation::Unload()
{
	mTextureID = 0;
}

void SpriteAnimation::Update(float deltaTime)
{
	if (mActive)
	{
		mTimer += mInfo.framePerSecond * deltaTime;
		if (!mInfo.looping && mTimer >= mInfo.frameCount)
			Stop();
	}
}

//void SpriteAnimation::Render(const Camera & camera)
//{
//	if (mActive)
//	{
//		int frameIndex = static_cast<int>(mTimer) % mInfo.frameCount;
//		int column = frameIndex % mInfo.columns;
//		ASSERT(mInfo.rows != 0, "Division by zero");
//		int row = frameIndex / mInfo.rows;
//		float frameWidth = JimmyGod::Graphics::TextureManager::Get()->GetTexture(mTextureID)->GetWidth / mInfo.columns;
//		float frameHeight = JimmyGod::Graphics::TextureManager::Get()->GetTexture(mTextureID)->GetHeight / mInfo.rows;
//		JimmyGod::Math::Rect rect;
//		rect.left = column * frameWidth;
//		rect.right = (column + 1) * frameWidth;
//		rect.top = row * frameHeight;
//		rect.bottom = (row + 1) * frameHeight;
//		SpriteRenderManager::Get()->DrawSprite(mTextureID, rect, camera.ConvertToScreenPosition(mPos));
//	}
//}

void SpriteAnimation::Render()
{
	if (mActive)
	{
		int frameIndex = static_cast<int>(mTimer) % mInfo.frameCount;
		int column = frameIndex % mInfo.columns;
		ASSERT(mInfo.rows != 0, "Division by zero");
		int row = frameIndex / mInfo.rows;
		float frameWidth = TextureManager::Get()->GetTexture(mTextureID)->GetWidth() / mInfo.columns;
		float frameHeight = TextureManager::Get()->GetTexture(mTextureID)->GetHeight() / mInfo.rows;
		JimmyGod::Math::Rect rect;
		rect.left = column * frameWidth;
		rect.right = (column + 1) * frameWidth;
		rect.top = row * frameHeight;
		rect.bottom = (row + 1) * frameHeight;
		SpriteRenderManager::Get()->DrawSprite(mTextureID, rect, mPos);
	}
}

void SpriteAnimation::Play()
{
	mTimer = 0.0f;
	mActive = true;
}

void SpriteAnimation::Stop()
{
	mActive = false;
}
