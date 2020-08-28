#include "Precompiled.h"
#include "MetaRegistration.h"

META_TYPE_DEFINE(int, Integer)
META_TYPE_DEFINE(float, Number)
META_TYPE_DEFINE(bool, Boolean)
META_TYPE_DEFINE(std::string, String)

void JimmyGod::Core::StaticMetaRegister()
{
	// Add MetaClass registration here..
	// e.g. META_REGISTER(Foo);
}