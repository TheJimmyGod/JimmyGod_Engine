#include "Precompiled.h"
#include "Entity.h"

#include "AIWorld.h"

using namespace JimmyGod::AI;

Entity::Entity(AIWorld & world, uint16_t typeId)
	: world(world), 
	mUniqueId((static_cast<uint64_t>(typeId) << 32)|world.GetNextId())
{
	world.RegisterEntity(this);
}

Entity::~Entity()
{
	world.UnregisterEntity(this);
}

JimmyGod::Math::Matrix3 JimmyGod::AI::Entity::LocalToWorld() const
{
	auto f = JimmyGod::Math::Normalize(Heading);
	auto s = JimmyGod::Math::PerpendicularRH(f);
	auto p = Position;
	return
	{
		s.x,s.y,0.0f,
		f.x,f.y,0.0f,
		p.x,p.y,1.0f
	};
}
