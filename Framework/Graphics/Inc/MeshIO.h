#pragma once

#include "Mesh.h"
#include "Material.h"

namespace JimmyGod::Graphics::MeshIO
{

	void Write(FILE* file, const SkinnedMesh& mesh);
	void Write(FILE* file, Material& material);
	void Read(FILE* file, SkinnedMesh& mesh);
	void Read(FILE* file, Material& material);
}

// Homework: Add MeshIO.cpp and implement Write/Read