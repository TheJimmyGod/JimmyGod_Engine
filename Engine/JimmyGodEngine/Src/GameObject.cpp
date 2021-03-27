#include "Precompiled.h"
#include "GameObject.h"

#include "Component.h"

using namespace JimmyGod;

META_CLASS_BEGIN(GameObject)
	META_NO_FIELD
META_CLASS_END

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

Component * GameObject::AddComponent(const Core::Meta::MetaClass * metaClass)
{
	Component* newComponent = static_cast<Component*>(metaClass->Create());
	newComponent->mOwner = this;
	mComponent.emplace_back(std::unique_ptr<Component>(newComponent));
	return newComponent;
}
