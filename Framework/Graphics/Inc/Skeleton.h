#pragma once

#include "Bone.h"
#include "AnimationClip.h"

namespace JimmyGod::Graphics
{
	struct Skeleton
	{
		Bone* root = nullptr;
		std::vector<std::unique_ptr<Bone>> bones;
	};

	Bone* FindBone(const Skeleton& skeleton, const char* boneName);
	void DrawSkeleton(Bone* bone, std::vector<Math::Matrix4>& boneMatrices, const Math::Vector3& modelPos, const Math::Vector3& scale , const Math::Matrix4& rot = Math::Matrix4::Identity);
	void UpdateBoneRecursive(Bone* bone, std::vector<Math::Matrix4>& boneMatrices);
	void UpdateBoneRecursive(Bone* bone, std::vector<Math::Matrix4>& boneMatrices, const AnimationClip& clip, float time);
}