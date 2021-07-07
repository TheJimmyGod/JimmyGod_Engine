#include "Precompiled.h"
#include "MetaRegistration.h"

#include "Component.h"
#include "GameObject.h"

// Components
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "ModelComponent.h"

using namespace JimmyGod;

void JimmyGod::StaticMetaRegister()
{
	// Add MetaClass registration here...
	// e.g. META_REGISTER(Foo);

	META_REGISTER(Component);
	META_REGISTER(GameObject);

	META_REGISTER(ColliderComponent);
	META_REGISTER(TransformComponent);
	META_REGISTER(ModelComponent);
}