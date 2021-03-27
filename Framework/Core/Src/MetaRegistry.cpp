#include "Precompiled.h"
#include "MetaRegistry.h"

#include "DebugUtil.h"
#include "MetaClass.h"

using namespace JimmyGod::Core;

namespace
{
	inline auto& GetRegistry()
	{
		using MetaClassMap = std::map<std::string, const Meta::MetaClass*>;
		static MetaClassMap sMetaClassMap;
		return sMetaClassMap;
	}
}

void Meta::Register(const MetaClass * metaClass)
{
	GetRegistry().emplace(metaClass->GetName(), metaClass);
}

const Meta::MetaClass * Meta::FindMetaClass(const char* className)
{
	auto iter = GetRegistry().find(className);
	ASSERT(iter != GetRegistry().end(), "MetaRegistry -- Meta class for %s not found.", className);
	return iter->second;
}
