#pragma once

#include "Colors.h"


namespace JimmyGod::Graphics
{
	
	struct Material
	{
		Color ambient;
		Color diffuse;
		Color specular;
		float power;
		float padding[3];
	};
}