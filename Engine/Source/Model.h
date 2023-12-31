#pragma once
#include <vector>
#include "glew-2.1.0/include/GL/glew.h"

namespace  tinygltf
{
	class Model;
}
class Mesh;

class Model {
public:

	void Load(const char* assetFileName);
	void LoadMaterials(const tinygltf::Model& srcModel);
	void CleanUp();
	void MenuConfigTexture();
	void Draw();

	 GLint wrap;
	 GLint mag;
	 GLint min;

private:
	std::vector<GLuint> texturesID;
	std::vector<Mesh*> meshes;

};