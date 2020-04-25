#include"Precompiled.h"
#include"Animation.h"


using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

Vector3 Animation::GetPosition(float time) const
{
	const int positionSize = mPositionKeys.size();
	Vector3 translationLerped;
	int currentKey = 0;
	int nextKey = 0;
	float total_t;
	float t;


	if (positionSize > 0)
	{
		total_t = 0.0f;
		t = 0.0f;

		for (int i = 0; i < positionSize; ++i)
		{
			currentKey = i;
			if (mPositionKeys[i].time > time)
				break;
		}
		nextKey = (currentKey + 1) % positionSize;
		total_t = mPositionKeys[nextKey].time - mPositionKeys[currentKey].time;
		if (total_t == 0)
			return Vector3::Zero;
		t = (time - mPositionKeys[currentKey].time) / total_t;

		Vector3 curr = mPositionKeys[currentKey].key;
		Vector3 next = mPositionKeys[nextKey].key;
		translationLerped = Lerp<Vector3>(curr, next, t);
	}
	return translationLerped;
}

Quaternion Animation::GetRotation(float time) const
{
	const int rotationSize = mRotationKeys.size();
	Quaternion Slerped;
	int currentKey = 0;
	int nextKey = 0;
	float total_t;
	float t;

	if (rotationSize > 0)
	{
		total_t = 0.0f;
		t = 0.0f;

		for (int i = 0; i < rotationSize; ++i)
		{
			currentKey = i;
			if (mPositionKeys[i].time > time)
				break;
		}
		nextKey = (currentKey + 1) % rotationSize;

		total_t = mRotationKeys[nextKey].time - mRotationKeys[currentKey].time;
		if (total_t == 0)
			return Quaternion::Zero;
		t = (time - mRotationKeys[currentKey].time) / total_t;

		Quaternion curr = mRotationKeys[currentKey].key;
		Quaternion next = mRotationKeys[nextKey].key;

		Slerped = Slerp(curr, next, time);
	}



	return Slerped;
}

Vector3 Animation::GetScale(float time) const
{
	const int scaleSize = mScaleKeys.size();
	Vector3 scaleLerped;
	int currentKey = 0;
	int nextKey = 0;
	float total_t;
	float t;

	if (scaleSize > 0)
	{
		total_t = 0.0f;
		t = 0.0f;

		for (int i = 0; i < scaleSize; ++i)
		{
			currentKey = i;
			if (mPositionKeys[i].time > time)
				break;
		}
		nextKey = (currentKey + 1) % scaleSize;
		total_t = mScaleKeys[nextKey].time - mScaleKeys[currentKey].time;
		if (total_t == 0)
			return Vector3::Zero;
		t = (time - mScaleKeys[currentKey].time) / total_t;

		Vector3 curr = mScaleKeys[currentKey].key;
		Vector3 next = mScaleKeys[nextKey].key;
		scaleLerped = Lerp<Vector3>(curr, next, t);
	}
	return scaleLerped;
}

Matrix4 Animation::GetTransform(float time) const
{
	Matrix4 translationVal = Matrix4::Translation(GetPosition(time));

	Matrix4 rotationVal = RotationQuaternion(GetRotation(time));

	Matrix4 scaleVal = Matrix4::Translation(GetScale(time));

	return translationVal * rotationVal * scaleVal;
}