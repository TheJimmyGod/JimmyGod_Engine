#include "Precompiled.h"
#include "Button.h"

using namespace JimmyGod;
using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

JimmyGod::Button::Button(const char* fileName_on, const char* fileName_off, const JimmyGod::Math::Vector2& pos, float radius)
{
	SetPosition(pos);
	mCollider = Circle(mPosition, radius);
	mTexture[0] = SpriteRenderManager::Get()->LoadTexture(fileName_on);
	mTexture[1] = SpriteRenderManager::Get()->LoadTexture(fileName_off);
}

void JimmyGod::Button::Update(float deltaTime)
{
	if (!IsDisplay)
		return;
	auto inputSyetem = JimmyGod::Input::InputSystem::Get();

	int MouseX = inputSyetem->GetMouseScreenX();
	int MouseY = inputSyetem->GetMouseScreenY();
	mCollider = Circle(mPosition, mCollider.radius);
	
	if (PointInCircle(Vector2(MouseX, MouseY), GetCollider()))
		onPoint = true;
	else
		onPoint = false;

	if (inputSyetem->IsMouseDown(MouseButton::LBUTTON))
	{
		if (PointInCircle(Vector2(MouseX, MouseY), GetCollider()) && !onClick)
		{
			Click(true);
			Invoke();
		}
	}
	else
		Click(false);
}

void JimmyGod::Button::Render()
{
	if (!IsDisplay) return;
	if (onPoint)
		SpriteRenderManager::Get()->DrawSprite(mTexture[0], mPosition);
	else
		SpriteRenderManager::Get()->DrawSprite(mTexture[1], mPosition);
	
}

void JimmyGod::Button::AddListener(std::function<void()> func)
{
	mFunc = std::move(func);
}

void JimmyGod::Button::Invoke() const
{
	if (!mFunc) return;
	mFunc();
}
