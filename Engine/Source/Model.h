#pragma once
#include <vector>
namespace  tinygltf
{
	class Model;
}
// Placeholder for Texture and Mesh classes
class Texture;
class Mesh;


class Model {
public:
	void Load(const char* assetFileName);
	void LoadMaterials(const tinygltf::Model& srcModel);
	void Draw();

private:
	std::vector<Texture*> textures;
	std::vector<Mesh*> meshes;

};