#include "Precompiled.h"
#include "SkeletonIO.h"

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics::SkeletonIO;

namespace
{
	void WriteMatrix(FILE* file, const Matrix4& m)
	{
		fprintf_s(file, "%f %f %f %f\n", m._11, m._12, m._13, m._14);
		fprintf_s(file, "%f %f %f %f\n", m._21, m._22, m._23, m._24);
		fprintf_s(file, "%f %f %f %f\n", m._31, m._32, m._33, m._34);
		fprintf_s(file, "%f %f %f %f\n", m._41, m._42, m._43, m._44);
	}
	void ReadMatrix(FILE* file, Matrix4& m)
	{
		fscanf_s(file, "%f %f %f %f\n", &m._11, &m._12, &m._13, &m._14);
		fscanf_s(file, "%f %f %f %f\n", &m._21, &m._22, &m._23, &m._24);
		fscanf_s(file, "%f %f %f %f\n", &m._31, &m._32, &m._33, &m._34);
		fscanf_s(file, "%f %f %f %f\n", &m._41, &m._42, &m._43, &m._44);
	}
}

void JimmyGod::Graphics::SkeletonIO::Write(FILE * file, const Skeleton & skeleton)
{
	const uint32_t size = static_cast<uint32_t>(skeleton.bones.size());
	fprintf_s(file, "BoneCount: %d\n", size);
	for (auto& bone : skeleton.bones)
	{
		fprintf_s(file, "Name: %s\n", bone->name.c_str());
		fprintf_s(file, "Index: %d\n", bone->index);
		fprintf_s(file, "ParentIndex: %d\n", bone->parentIndex);
		fprintf_s(file, "ChildCount: %d\n", bone->children.size());
		for (auto& children : bone->children)
		{
			fprintf_s(file, "ChildIndices: %d\n", children->index);
		}
		
		WriteMatrix(file, bone->toParentTransform);
		WriteMatrix(file, bone->offsetTransform);
	}	
}

void JimmyGod::Graphics::SkeletonIO::Read(FILE * file, Skeleton & skeleton)
{
	for (size_t i = 0; i < skeleton.bones.size(); i++)
	{
		auto bone = std::make_unique<Bone>();
		char name[128];
		fscanf_s(file, "Name: %s\n", &name, sizeof(name));
		bone->name = name;
		fscanf_s(file, "Index: %d\n", &bone->index);
		fscanf_s(file, "ParentIndex: %d\n", &bone->parentIndex);

		uint32_t count = 0;
		fscanf_s(file, "ChildCount: %d\n", &count);
		for (uint32_t i = 0; i < count; ++i)
		{
			uint32_t index = 0;
			fscanf_s(file, "ChildIndices: %d\n", &index);
			bone->childIndice.push_back(index);
		}
		ReadMatrix(file, bone->toParentTransform);
		ReadMatrix(file, bone->offsetTransform);

		skeleton.bones[i] = std::move(bone);
	}

}
