#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE

#include "Model.h"
#include "Globals.h"
#include "Application.h"
#include "Mesh.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"

#include "TinyGLTF/tiny_gltf.h"

bool Model::Init() {
	return true;
}
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
			meshes.push_back(mesh);
		}
	}
	LoadMaterials(model);
}

void Model::LoadMaterials(const tinygltf::Model& srcModel)
{
	for (const auto& srcMaterial : srcModel.materials)
	{
		Texture textureMaterial;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = srcModel.images[texture.source];
			textureMaterial = App->GetTexture()->Load(image.uri, GL_REPEAT, GL_NEAREST, GL_LINEAR, false);
		}
		textures.push_back(textureMaterial);
	}
}

void Model::Draw()
{
	Mesh* mesh = new Mesh;
	float4x4 modelMatrix = App->GetCamera()->GetModel();

	for (const auto& mesh : meshes) 
	{
		mesh->Render(textures);
	}
}
