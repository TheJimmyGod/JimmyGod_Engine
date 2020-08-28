#include "Precompiled.h"
#include "GameObject.h"

#include "Component.h"

using namespace JimmyGod;

void GameObject::Initialize()
{
	ASSERT(!mInitialized, "GameObject -- Game object alread initialized!");

	for (auto& component : mComponent)
		component->Initialize();
	mInitialized = true;
}
void GameObject::Terminate()
{
	for (auto& component : mComponent)
		component->Terminate();
}
void GameObject::Update(float deltaTime)
{
	for (auto& component : mComponent)
		component->Update(deltaTime);
}
void GameObject::Render()
{
	for (auto& component : mComponent)
		component->Render();
}
void GameObject::DebugUI()
{
	for (auto& component : mComponent)
		component->DebugUI();
}