#pragma once

namespace JimmyGod::Math
{
	struct Ray
	{
		Vector3 origin = Vector3::Zero;
		Vector3 direction = Vector3::ZAxis;

		const Vector3 GetPoint(float s) const
		{
			return origin + direction * s;
		}
	};
}