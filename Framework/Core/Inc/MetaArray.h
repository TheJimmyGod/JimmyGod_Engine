#pragma once

#include "MetaType.h"

namespace JimmyGod::Core::Meta
{
	class MetaArray : public MetaType
	{
	public:
		MetaArray(const MetaType* elementType);

		const MetaType* GetElementType() const { return mElementType; }
	private:
		const MetaType* const mElementType;
	};
}