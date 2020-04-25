#include <Graphics/Inc/Graphics.h>
#include <Math/Inc/EngineMath.h>

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <cstdio>

int main(int argc, char* argv[])
{
	// Create an instance of the importer class to do the parsing for us
	Assimp::Importer importer;
	printf("All done!\n");
	return 0;
}