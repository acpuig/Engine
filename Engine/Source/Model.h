#pragma once
#include<list>
#include "glew-2.1.0/include/GL/glew.h"
#include <vector>
#include <string>

namespace  tinygltf
{
	class Model;
}
class Model {
public:
	void Load(const char* assetFileName);
	void LoadMaterials(const tinygltf::Model& srcModel);

private:
	//std::list<Texture*> modules;

};