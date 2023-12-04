#pragma once
#include <vector>
#include "ModuleTexture.h"

namespace  tinygltf
{
	class Model;
}
// Placeholder for Texture and Mesh classes
class Texture;
class Mesh;


class Model {
public:
	bool Init();
	void Load(const char* assetFileName);
	void LoadMaterials(const tinygltf::Model& srcModel);
	void Draw();

private:
	std::vector<Texture> textures;
	std::vector<Mesh*> meshes;

};