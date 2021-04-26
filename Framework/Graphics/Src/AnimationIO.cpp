#include "Precompiled.h"
#include "AnimationIO.h"

using namespace JimmyGod;
using namespace JimmyGod::Graphics;

void AnimationIO::Write(FILE* file, const Animation& animation)
{
	// Homework
	fprintf_s(file, "PositionKeyCount: %d\n", animation.mPositionKeys.size());
	for (uint32_t i = 0; i < animation.mPositionKeys.size(); ++i)
	{
		fprintf_s(file, "%f %f %f %f\n",
			animation.mPositionKeys[i].key.x,
			animation.mPositionKeys[i].key.y,
			animation.mPositionKeys[i].key.z,
			animation.mPositionKeys[i].time);
	}
	fprintf_s(file, "RotationKeyCount: %d\n", animation.mRotationKeys.size());
	for (uint32_t i = 0; i < animation.mRotationKeys.size(); ++i)
	{
		fprintf_s(file, "%f %f %f %f %f\n",
			animation.mRotationKeys[i].key.x,
			animation.mRotationKeys[i].key.y,
			animation.mRotationKeys[i].key.z,
			animation.mRotationKeys[i].key.w,
			animation.mRotationKeys[i].time);
	}
	fprintf_s(file, "ScaleKeyCount: %d\n", animation.mScaleKeys.size());
	for (uint32_t i = 0; i < animation.mScaleKeys.size(); ++i)
	{
		fprintf_s(file, "%f %f %f %f\n",
			animation.mScaleKeys[i].key.x,
			animation.mScaleKeys[i].key.y,
			animation.mScaleKeys[i].key.z,
			animation.mScaleKeys[i].time);
	}
}

void AnimationIO::Read(FILE* file, Animation& animation)
{
	float x, y, z, w, t;
	uint32_t size = static_cast<uint32_t>(animation.mPositionKeys.size());
	fscanf_s(file, "PositionKeyCount: %d\n", &size);
	animation.mPositionKeys.resize(size);
	for (uint32_t i = 0; i < size; ++i)
	{
		fscanf_s(file, "%f %f %f %f\n",&x,&y,&z,&t);

		animation.mPositionKeys[i].key.x = x;
		animation.mPositionKeys[i].key.y = y;
		animation.mPositionKeys[i].key.z = z;
		animation.mPositionKeys[i].time = t;
	}
	size = static_cast<uint32_t>(animation.mRotationKeys.size());
	fscanf_s(file, "RotationKeyCount: %d\n", &size);
	animation.mRotationKeys.resize(size);
	for (uint32_t i = 0; i < size; ++i)
	{
		fscanf_s(file, "%f %f %f %f %f\n",&x,&y,&z,&w,&t);

		animation.mRotationKeys[i].key.x = x;
		animation.mRotationKeys[i].key.y = y;
		animation.mRotationKeys[i].key.z = z;
		animation.mRotationKeys[i].key.w = w;
		animation.mRotationKeys[i].time = t;
	}
	size = static_cast<uint32_t>(animation.mScaleKeys.size());
	fscanf_s(file, "ScaleKeyCount: %d\n", &size);
	animation.mScaleKeys.resize(size);
	for (uint32_t i = 0; i < size; ++i)
	{
		fscanf_s(file, "%f %f %f %f\n",&x,&y,&z,&t);

		animation.mScaleKeys[i].key.x = x;
		animation.mScaleKeys[i].key.y = y;
		animation.mScaleKeys[i].key.z = z;
		animation.mScaleKeys[i].time = t;
	}
	// Homework
}

void AnimationIO::Write(FILE* file, const AnimationClip& animationClip)
{
	// Homework
	// Note that animationClip has the same number of animations as
	// there are bones. This is so that we can use the bone index
	// to index for an animation directly. If animationClip.boneAnimation[i]
	// is null, you will still need to write out something like [Empty]
	// so when you load the animation clip, you know to skip those slots

	// boneAnimations:
	// [--->][null][--->][--->][null]
	//
	// output:
	// Animation : ...
	// [Empty]
	// Animation : ...
	// Animation : ...
	// Animation : ...
	// [Empty]

	fprintf_s(file, "Animation: %s\n", animationClip.name.c_str());
	fprintf_s(file, "Duration: %f\n", animationClip.duration);
	fprintf_s(file, "TickPerSecond: %f\n", animationClip.ticksPerSecond);
	fprintf_s(file, "BoneAnimationCount: %d\n", static_cast<uint32_t>(animationClip.boneAnimations.size()));
	for (auto& clips : animationClip.boneAnimations)
	{
		if (clips)
		{
			fprintf_s(file, "Animation: \n");
			Write(file, *clips);
		}
		else
		{
			fprintf_s(file, "[Empty]\n");
		}
	}

}

void AnimationIO::Read(FILE* file, AnimationClip& animationClip)
{
	char name[256];
	fscanf_s(file, "Animation: %s\n", &name, sizeof(name));
	animationClip.name = name;
	fscanf_s(file, "Duration: %f\n", &animationClip.duration);
	fscanf_s(file, "TickPerSecond: %f\n", &animationClip.ticksPerSecond);

	uint32_t boneAnimationCout = 0;
	fscanf_s(file, "BoneAnimationCount: %d\n", &boneAnimationCout);
	animationClip.boneAnimations.resize(boneAnimationCout);
	for (uint32_t i = 0; i < boneAnimationCout; ++i)
	{
		char empty[20];
		fscanf_s(file, "%s\n", &empty, sizeof(empty));
		if (std::string(empty) == "[Empty]")
		{
			continue;
		}
		auto boneAnimation = std::make_unique<Animation>();
		animationClip.boneAnimations[i] = std::move(boneAnimation);
		Read(file, *animationClip.boneAnimations[i]);
	}
	// Homework
}