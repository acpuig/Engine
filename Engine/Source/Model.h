#pragma once
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE

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

};