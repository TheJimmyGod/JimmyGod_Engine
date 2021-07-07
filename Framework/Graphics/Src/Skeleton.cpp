#include "Precompiled.h"
#include "Skeleton.h"

#include "SimpleDraw.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;

namespace
{
	Matrix4 GetTransform(const Bone& b)
	{
		return b.toParentTransform;
	}
}


Bone* JimmyGod::Graphics::FindBone(const Skeleton& skeleton, const char* boneName)
{
	for (auto& bone : skeleton.bones)
	{
		if (bone->name == boneName)
			return bone.get();
	}
	return nullptr;
}

void JimmyGod::Graphics::DrawSkeleton(Bone* bone, std::vector<Math::Matrix4>& boneMatrices,const Math::Vector3& modelPos, const Math::Vector3& scale, const Math::Matrix4& rot)
{
	if (bone == nullptr)
		return;

	// Homework
	// Use skeleton so you know what the parent child order is
	// But, use boneMatrices, which is the multipled out matrices, to get the position
	// Draw lines to connect the bones
	// ex) Bone4 = Bone4.toParent * bones[Bone4.parentIndex].toParent * ...

	// Translation Matrix by dx, dy, dz
	// 1,0,0,0
	// 0,1,0,0
	// 0,0,1,0
	// x,y,z,1

	if (bone->parent != nullptr)
	{
		auto myMatrix = boneMatrices[bone->index] * rot;
		auto parentMatrix = boneMatrices[bone->parent->index] * rot;
		auto myPos = GetTranslation(myMatrix);
		auto parentPos = GetTranslation(parentMatrix);

		SimpleDraw::AddLine((modelPos)+myPos * scale, (modelPos)+parentPos * scale, Colors::Aqua);
		SimpleDraw::AddSphere((modelPos)+myPos * scale, 0.05f, Colors::Gold, 2, 2);
	}

	for (auto& child : bone->children)
		DrawSkeleton(child, boneMatrices, modelPos, scale,rot);
}

void JimmyGod::Graphics::UpdateBoneRecursive(Bone* bone, std::vector<Math::Matrix4>& boneMatrices)
{
	if (bone == nullptr)
		return;

	if (bone->parent == nullptr)
	{
		boneMatrices[bone->index] = Matrix4::Identity;
	}
	else
	{
		boneMatrices[bone->index] = bone->toParentTransform * boneMatrices[bone->parent->index];
	}
	for (auto& c : bone->children)
		UpdateBoneRecursive(c, boneMatrices);

}

void JimmyGod::Graphics::UpdateBoneRecursive(Bone* bone, std::vector<Math::Matrix4>& boneMatrices, const AnimationClip & clip, float time)
{
	if (bone == nullptr)
		return;
	Matrix4 transform = bone->toParentTransform;
	clip.GetTransform(time, bone->index, transform);
	if (bone->parent != nullptr)
	{
		boneMatrices[bone->index] = transform * boneMatrices[bone->parent->index];
	}
	else
	{
		boneMatrices[bone->index] = Matrix4::Identity;
	}
	for (auto& c : bone->children)
		UpdateBoneRecursive(c, boneMatrices,clip,time);
}
