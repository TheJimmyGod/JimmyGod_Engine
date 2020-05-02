#pragma once

#include "Mesh.h"

namespace JimmyGod::Graphics::MeshIO
{
	void Write(FILE* file, const Mesh& mesh);
	void Read(FILE* file, Mesh& mesh);
}

// Homework: Add MeshIO.cpp and implement Write/Read