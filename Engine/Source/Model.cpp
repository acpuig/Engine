#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE

#include "Model.h"; 
#include "Globals.h"
#include "Application.h"
#include "Mesh.h"
#include "ModuleTexture.h"

#include "TinyGLTF/tiny_gltf.h"


void Model::Load(const char* assetFileName)
{
	assert(assetFileName != nullptr);
	tinygltf::TinyGLTF gltfContext;
	tinygltf::Model model;
	std::string error, warning;
	bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, assetFileName);
	if (!loadOk)
	{
		LOG("Error loading %s: %s", assetFileName, error.c_str());
	}

	for (const auto& srcMesh : model.meshes)
	{
		for (const auto& primitive : srcMesh.primitives)
		{
			Mesh* mesh = new Mesh;
			mesh->Load(model, srcMesh, primitive);
		}
	}


}

void Model::LoadMaterials(const tinygltf::Model& srcModel)
{
	for (const auto& srcMaterial : srcModel.materials)
	{
		Texture textureId;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = srcModel.images[texture.source];
			textureId = App->GetTexture()->Load(image.uri, GL_REPEAT, GL_NEAREST, GL_LINEAR, false);
		}
		textures.push_back(&textureId);
	}
}

void Model::Draw()
{
	tinygltf::Model model;

	for (const auto& srcMesh : model.meshes)
	{
		for (const auto& srcMesh : srcMesh.primitives)
		{
			Mesh* mesh = new Mesh;
			//mesh->Render();

		}
	}
}
