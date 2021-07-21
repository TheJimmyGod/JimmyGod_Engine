#include "Precompiled.h"
#include "AgentComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::AI;

META_DERIVED_BEGIN(AgentComponent, Component)
	META_NO_FIELD
META_CLASS_END