#include"Precompiled.h"
#include"Animation.h"


using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

Vector3 Animation::GetPosition(float time) const
{
	const int positionSize = mPositionKeys.size();
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
			t = time - mPositionKeys[currentKey].time;
		else
			t = (time - mPositionKeys[currentKey].time) / total_t;

		Vector3 curr = mPositionKeys[currentKey].key;
		Vector3 next = mPositionKeys[nextKey].key;
		return Lerp<Vector3>(curr, next, t);
	}
	return Vector3::One;
}

Quaternion Animation::GetRotation(float time) const
{
	const int rotationSize = mRotationKeys.size();
	Quaternion Slerped = Quaternion::Identity;
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
			if (mRotationKeys[i].time > time)
				break;
		}
		nextKey = (currentKey + 1) % rotationSize;

		total_t = mRotationKeys[nextKey].time - mRotationKeys[currentKey].time;
		if (total_t == 0)
			t = time - mRotationKeys[currentKey].time;
		else
			t = (time - mRotationKeys[currentKey].time) / total_t;

		Quaternion curr = mRotationKeys[currentKey].key;
		Quaternion next = mRotationKeys[nextKey].key;

		return Slerp(curr, next, t);
	}
	return Quaternion::Identity;
}

Vector3 Animation::GetScale(float time) const
{
	const int scaleSize = mScaleKeys.size();
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
			if (mScaleKeys[i].time > time)
				break;
		}
		nextKey = (currentKey + 1) % scaleSize;
		total_t = mScaleKeys[nextKey].time - mScaleKeys[currentKey].time;
		if (total_t == 0)
			t = time - mScaleKeys[currentKey].time;
		else
			t = (time - mScaleKeys[currentKey].time) / total_t;

		Vector3 curr = mScaleKeys[currentKey].key;
		Vector3 next = mScaleKeys[nextKey].key;
		return Lerp<Vector3>(curr, next, t);
	}
	return Vector3::One;
}

Matrix4 Animation::GetTransform(float time) const
{
	Matrix4 translationVal = Matrix4::Translation(GetPosition(time));

	Matrix4 rotationVal = Matrix4::RotationQuaternion(GetRotation(time));

	Matrix4 scaleVal = Matrix4::Scaling(GetScale(time));

	return translationVal * rotationVal * scaleVal;
}
