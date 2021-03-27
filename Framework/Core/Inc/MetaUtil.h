#pragma once

namespace JimmyGod::Core::Meta
{
	class MetaType;

	template<class DataType>
	const MetaType* GetMetaType();

	template <class DataType>
	void Deserialize(void* instance, const rapidjson::Value& value)
	{
		static_assert(sizeof(DataType) == -1, "No specialization found for deserializing this type.");
	}

	namespace Detail
	{
		template <class DataType>
		inline const MetaType* GetMetaTypeImpl(DataType*)
		{
			static_assert(sizeof(DataType) == -1, "No Metatype found for this");
		}
		template <class DataType>
		inline const MetaType* GetMetaTypeImpl(DataType**)
		{
			static const MetaPointer sMetaPointer(GetMetaType<DataType>());
			return &sMetaPointer;
		}
		template <class DataType>
		inline const MetaType* GetMetaTypeImpl(std::vector<DataType>*)
		{
			static const MetaArray sMetaArray(GetMetaType<DataType>());
			return &sMetaArray;
		}
	}

	template <class DataType>
	inline const MetaType* GetMetaType()
	{
		return Detail::GetMetaTypeImpl(static_cast<DataType*>(nullptr));
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