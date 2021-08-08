#include "Unit.h"
#include "Flag.h"
#include "GridManager.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;

Unit::Unit(std::string name, Flag flag, float health, float dmg, float def, float range) :
	mName(name), mFlag(flag), mHealth(health), mDamage(dmg), mDefence(def), mRange(range)
{
	mMaxHelath = health;
}

void Unit::Initialize(JimmyGod::GameWorld* gameWorld, const std::filesystem::path& path)
{
	ASSERT(gameWorld != nullptr, "The Game World does not exist!");
	gameWorld->Create(path, mName);
	mGameObject = gameWorld->Find(mName).Get();
	mTransformBuffer.Initialize();
}

void Unit::Terminate()
{
	mTransformBuffer.Terminate();
	mGameObject->Terminate();
}

void Unit::Update(float deltaTime)
{
	if (isDead)
		return;
	if (mGameObject->GetComponent<AgentComponent>()->GetSpeed() > 0.1f)
		mGameObject->GetComponent<ModelComponent>()->PlayAnimation(1);
	else
		mGameObject->GetComponent<ModelComponent>()->PlayAnimation(0);
}

void Unit::Render(const JimmyGod::Graphics::Camera& camera)
{
	if (isDead)
		return;
	auto matView = camera.GetViewMatrix();
	auto matProj = camera.GetPerspectiveMatrix();
	auto matWorld = mGameObject->GetComponent<TransformComponent>()->GetTransform();
	mTransformBuffer.BindVS(0);
	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	transformData.viewPosition = camera.GetPosition();
	mTransformBuffer.Update(&transformData);

	mGameObject->GetComponent<ModelComponent>()->Render();
}

void Unit::DebugUI()
{
}

void Unit::Move(const JimmyGod::AI::Coord& pos)
{
	if (GridManager::Get() == nullptr)
		return;
	JimmyGod::AI::Coord destiniation = pos;
	JimmyGod::AI::Coord current = GridManager::Get()->GetGraph().GetCoordinate(
		mGameObject->GetComponent<AgentComponent>()->GetPosition());
	GridManager::Get()->GetGird().FindPath(current,
		destiniation, mGameObject->GetComponent<AgentComponent>()->mArea,
		mGameObject->GetComponent<AgentComponent>()->mPath);
	if (mGameObject->GetComponent<AgentComponent>()->mPath.size() > 0)
		mGameObject->GetComponent<AgentComponent>()->ChangeState("Move");
}
