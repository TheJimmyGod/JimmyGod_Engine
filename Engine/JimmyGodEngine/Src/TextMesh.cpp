#include "Precompiled.h"
#include "TextMesh.h"

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

JimmyGod::TextMesh::TextMesh(const char* str, const JimmyGod::Math::Vector3& pos, float size, JimmyGod::Graphics::Color color, uint32_t animate, float lifeTime)
	: mString(str), mPosition(pos), mSize(size), mColor(color), mLifeTime(lifeTime)
{
	mCurrentAnimate = AnimateText{ animate };
	mMaxLifeTime = lifeTime;
	mIntialized = true;
}

void JimmyGod::TextMesh::Initialize(const char* str, const JimmyGod::Math::Vector3& pos, float size, JimmyGod::Graphics::Color color, uint32_t animate, float lifeTime)
{
	mString = str;
	mPosition = pos;
	mSize = size;
	mColor = color;
	mLifeTime = lifeTime;
	mMaxLifeTime = lifeTime;
	mCurrentAnimate = AnimateText{animate};
	mIntialized = true;
}

void JimmyGod::TextMesh::Update(const JimmyGod::Graphics::Camera& camera, float deltaTime)
{
	
	if (mIntialized == false)
		return;

	if (mCurrentAnimate == JimmyGod::TextMesh::Stand)
	{
		Vector2 convertedPos = camera.ConvertTo2DSpace(mPosition);
		SpriteRenderManager::Get()->DrawScreenText(mString.c_str(), convertedPos.x, convertedPos.y, (mSize - mLocation * 50 < 1 ? 1 : mSize - mLocation * 50), mColor);
		return;
	}

	auto graphicsSyetem = JimmyGod::Graphics::GraphicsSystem::Get();
	if (mLifeTime > 0.0f)
	{
		mLifeTime -= deltaTime;
		mLocation += deltaTime;
		mColor = Lerp(mColor, Vector4{ 0.0f }, deltaTime);
		Vector2 convertedPos = camera.ConvertTo2DSpace(mPosition);
		switch (mCurrentAnimate)
		{
		case JimmyGod::TextMesh::Up:
			SpriteRenderManager::Get()->DrawScreenText(mString.c_str(),convertedPos.x, convertedPos.y - (100.0f * mLocation), (mSize - mLocation * 50 < 1 ? 1 : mSize - mLocation * 50), mColor);
			break;
		case JimmyGod::TextMesh::Down:
			SpriteRenderManager::Get()->DrawScreenText(mString.c_str(), convertedPos.x,  convertedPos.y + (100.0f * mLocation), (mSize - mLocation * 50 < 1 ? 1 : mSize - mLocation * 50), mColor);
			break;
		case JimmyGod::TextMesh::Right:
			SpriteRenderManager::Get()->DrawScreenText(mString.c_str(), convertedPos.x + (100.0f * mLocation), convertedPos.y, (mSize - mLocation * 50 < 1 ? 1 : mSize - mLocation * 50), mColor);
			break;
		case JimmyGod::TextMesh::Left:
			SpriteRenderManager::Get()->DrawScreenText(mString.c_str(),  convertedPos.x - (100.0f * mLocation), convertedPos.y, (mSize - mLocation * 50 < 1 ? 1 : mSize - mLocation * 50), mColor);
			break;
		case JimmyGod::TextMesh::None:
			SpriteRenderManager::Get()->DrawScreenText(mString.c_str(), convertedPos.x, convertedPos.y, (mSize - mLocation * 50 < 1 ? 1 : mSize - mLocation * 50) , mColor);
			break;
		case JimmyGod::TextMesh::Stand:
			break;
		default:
			break;
		}
	}
}
