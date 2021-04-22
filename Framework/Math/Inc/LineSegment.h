#pragma once
#include "Vector2.h"

namespace JimmyGod::Math
{
	struct LineSegment
	{
		Vector2 from;
		Vector2 to;

		LineSegment() : from(0.0f, 0.0f), to(0.0f, 0.0f) {}
		LineSegment(float x0, float y0, float x1, float y1) : from(x0, y0), to(x1, y1) {}
		LineSegment(const Vector2& v0, const Vector2& v1) : from(v0), to(v1) {}

		//float Length() const
		//{
		//	return sqrt(pow(to.x - from.x, 2) + pow(to.y - from.y, 2));
		//}

		//Vector2 midPoint() const
		//{
		//	return Vector2(0.5f * (to.x + from.x), 0.5f*(to.y + from.y));
		//}
	};
}