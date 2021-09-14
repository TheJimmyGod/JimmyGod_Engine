#include "Precompiled.h"
#include "Button.h"

using namespace JimmyGod;
using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

JimmyGod::Button::Button(const char* fileName, const JimmyGod::Math::Vector2& pos, float radius)
{
	SetPosition(pos);
	mCollider = Circle(mPosition, radius);
	mTexture = SpriteRenderManager::Get()->LoadTexture(fileName);
}

void JimmyGod::Button::Update(float deltaTime)
{
	if (!IsDisplay)
		return;
	auto inputSyetem = JimmyGod::Input::InputSystem::Get();

	int MouseX = inputSyetem->GetMouseScreenX();
	int MouseY = inputSyetem->GetMouseScreenY();
	mCollider = Circle(mPosition, mCollider.radius);
	
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
	SpriteRenderManager::Get()->DrawSprite(mTexture, mPosition);
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
