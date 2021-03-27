#pragma once
#include "MetaArray.h"
#include "MetaClass.h"
#include "MetaField.h"
#include "MetaPointer.h"
#include "MetaRegistry.h"
#include "MetaType.h"
#include "MetaUtil.h"

#define META_TYPE_DECLARE(Type)\
	template<> const JimmyGod::Core::Meta::MetaType* JimmyGod::Core::Meta::GetMetaType<Type>();

#define META_TYPE_DEFINE(Type, Name)\
	template<> const JimmyGod::Core::Meta::MetaType* JimmyGod::Core::Meta::GetMetaType<Type>()\
	{\
		static const JimmyGod::Core::Meta::MetaType sMetaType(\
			JimmyGod::Core::Meta::MetaType::Category::Primitive, #Name, sizeof(Type),\
			JimmyGod::Core::Meta::Deserialize<Type>);\
		return &sMetaType;\
	}

#define META_CLASS_DECLARE\
	static const JimmyGod::Core::Meta::MetaClass* StaticGetMetaClass();\
	virtual const JimmyGod::Core::Meta::MetaClass* GetMetaClass() const { return StaticGetMetaClass(); }

#define META_CLASS_BEGIN(ClassType)\
	META_CLASS_BEGIN_INTERNAL(ClassType)\
		const JimmyGod::Core::Meta::MetaClass* parentMetaClass = nullptr;

#define META_DERIVED_BEGIN(ClassType, ParentType)\
	META_CLASS_BEGIN_INTERNAL(ClassType)\
		const JimmyGod::Core::Meta::MetaClass* parentMetaClass = ParentType::StaticGetMetaClass();

#define META_CLASS_BEGIN_INTERNAL(ClassType)\
	template <> const JimmyGod::Core::Meta::MetaType* JimmyGod::Core::Meta::GetMetaType<ClassType>()\
	{\
		return ClassType::StaticGetMetaClass();\
	}\
	const JimmyGod::Core::Meta::MetaClass* ClassType::StaticGetMetaClass()\
	{\
		using Class = ClassType;\
		const char* className = #ClassType;


#define META_FIELD_BEGIN\
		static const std::initializer_list<JimmyGod::Core::Meta::MetaField> fields {

#define META_FIELD(Var, Name)\
			{ JimmyGod::Core::Meta::GetFieldType(&Class::Var), Name, JimmyGod::Core::Meta::GetFieldOffset(&Class::Var) },


#define META_FIELD_END\
		};

#define META_NO_FIELD\
		static const std::initializer_list<JimmyGod::Core::Meta::MetaField> fields;


#define META_CLASS_END\
		static const JimmyGod::Core::Meta::MetaClass sMetaClass(\
			className, sizeof(Class),\
			parentMetaClass, fields,\
			[] { return new Class(); }); \
		return &sMetaClass;\
	}
