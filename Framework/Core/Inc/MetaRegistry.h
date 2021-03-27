#pragma once

namespace JimmyGod::Core::Meta
{
	class MetaClass;

	// Register meta class so we can look it up later by name
	void Register(const MetaClass* metaClass);

	// Look up meta class by name
	const MetaClass* FindMetaClass(const char* className);
}

#define META_REGISTER(Class)\
	JimmyGod::Core::Meta::Register(Class::StaticGetMetaClass())