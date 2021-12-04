#pragma once

#include "GridManager.h"
#include "Environment.h"
#include "Flag.h"
#include "Unit.h"

namespace JimmyCom
{
	class Grass : public Environment
	{
	public:
		Grass(std::string name, bool destructible, const JimmyGod::Math::Vector3& pos, JimmyGod::GameWorld* gameWorld) : Environment(name, destructible)
		{
			mHealth = 1.0f;

			ASSERT(gameWorld != nullptr, "The Game World does not exist!");
			gameWorld->Create("../../Assets/Templates/Grass.json", mName);
			mGameObject = gameWorld->Find(mName).Get();

			mGameObject->GetComponent<TransformComponent>()->SetPosition(pos);
			LOG("Grass - L-value!");
		}

		Grass(std::string name, bool destructible, JimmyGod::Math::Vector3&& pos, JimmyGod::GameWorld* gameWorld) : Environment(name, destructible)
		{
			mHealth = 1.0f;

			ASSERT(gameWorld != nullptr, "The Game World does not exist!");
			gameWorld->Create("../../Assets/Templates/Grass.json", mName);
			mGameObject = gameWorld->Find(mName).Get();

			mGameObject->GetComponent<TransformComponent>()->SetPosition(pos);
			LOG("Grass - R-value!");
		}

		void Render(const JimmyGod::Graphics::Camera& camera) override
		{
			if (!mActive) return;

			auto matView = camera.GetViewMatrix();
			auto matProj = camera.GetPerspectiveMatrix();

			auto matWorld = mGameObject->GetComponent<TransformComponent>()->GetTransform();
			mGameObject->GetComponent<MeshComponent>()->Bind();
			mTransformBuffer.BindVS(0);
			TransformData transformData;
			transformData.world = Transpose(matWorld);
			transformData.wvp = Transpose(matWorld * matView * matProj);
			transformData.viewPosition = camera.GetPosition();
			mTransformBuffer.Update(&transformData);
			mGameObject->GetComponent<MeshComponent>()->Render();
		}

		void InstallGrid() override
		{
			
		}
	};
}