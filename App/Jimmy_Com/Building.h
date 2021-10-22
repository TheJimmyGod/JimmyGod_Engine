#pragma once

#include "GridManager.h"
#include "Environment.h"
#include "Flag.h"
#include "Unit.h"

namespace JimmyCom
{
	class Building : public Environment
	{
	public:
		Building(std::string name, bool destructible) : Environment(name, destructible)
		{
			mHealth = 100.0f;
		}

		void Initialize(JimmyGod::GameWorld* gameWorld) override
		{
			ASSERT(gameWorld != nullptr, "The Game World does not exist!");
			gameWorld->Create("../../Assets/Templates/Building.json", mName);
			mGameObject = gameWorld->Find(mName).Get();
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
			AI::Coord coord = GridManager::Get()->GetGrid().GetGraph().GetNode(mGameObject->GetComponent<TransformComponent>()->GetPosition())->coordinate;
			int minX, maxX, minY, maxY;
			minY = Max(coord.y - static_cast<int>(GetTerritory()- 1), 1);
			minX = Max(coord.x - static_cast<int>(GetTerritory() - 1), 1);
			maxY = Min(coord.y + static_cast<int>(GetTerritory()), GridManager::Get()->GetGrid().GetRows());
			maxX = Min(coord.x + static_cast<int>(GetTerritory()), GridManager::Get()->GetGrid().GetColumns());

			for (int y = minY; y < maxY; y++)
			{
				for (int x = minX; x < maxX; x++)
				{
					const int index = GridManager::Get()->GetGird().GetIndex(x, y);
					if (GridManager::Get()->GetGird().GetGraph().GetNode(AI::Coord{ x,y }))
					{
						GridManager::Get()->GetGird().GetNodes()[index].SetWalkable(false);
					}
				}
			}
		}
	};
}