#pragma once

namespace JimmyGod::Core::Meta
{
	class MetaType;

	template<class DataType>
	const MetaType* GetMetaType();

	namespace Detail
	{
		template <class DataType>
		inline const MetaType* GetMetaType(DataType*)
		{
			static_assert(sizeof(DataType) == -1, "No Metatype found for this");
		}
	}

	template <class DataType>
	inline const MetaType* GetMetaType()
	{
		return Detail::GetMetaType(static_cast<DataType*>(nullptr));
	}

	template <class ClassType, class DataType>
	inline const MetaType* GetFieldType(DataType ClassType::*)
	{
		return GetMetaType<DataType>();
	}

	template <class ClassType, class DataType>
	inline size_t GetFieldOffset(DataType ClassType::* field)
	{
		return (size_t)(void*)&(((ClassType*)nullptr)->*field);
	}



	// SFINAE - Substitution Failure Is Not An Error
}