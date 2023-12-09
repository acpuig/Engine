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
	LoadMaterials(model);
	
	for (const auto& srcMesh : model.meshes)
	{
		for (const auto& primitive : srcMesh.primitives)
		{
			//if(model.meshes[0] == srcMesh){
				Mesh* mesh = new Mesh;
				mesh->Load(model, srcMesh, primitive);
				meshes.push_back(mesh);
				//mesh->CleanUp();

			//}
		}
	}
//	for (const auto& srcMesh : model.meshes)
//	{
//		for (const auto& primitive : srcMesh.primitives)
	//	{
	//		if (model.meshes[1] == srcMesh) {
		//		Mesh* mesh = new Mesh;
		//		mesh->Load(model, srcMesh, primitive);
		//		meshes.push_back(mesh);
				//mesh->CleanUp()
			//}
		//}
	//}
}

void Model::Draw()
{
	if (!meshes.empty())
	{
		for(GLuint textureMaterialID : texturesID){
			for(Mesh* mesh : meshes){
				mesh->Render(textureMaterialID);
				float4x4 modelMatrix = App->GetCamera()->GetModel();
			}
		}
	}
}

void Model::LoadMaterials(const tinygltf::Model& srcModel)
{
	for (const auto& srcMaterial : srcModel.materials)
	{
		GLuint textureMaterialID;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = srcModel.images[texture.source];
			textureMaterialID = App->GetTexture()->Load(image.uri, GL_REPEAT, GL_NEAREST, GL_LINEAR, true);
		}
		texturesID.push_back(textureMaterialID);
	}
}

void Model::CleanUp() {
	for (Mesh* mesh : meshes) {
		mesh->CleanUp(); 
	}
	meshes.clear(); 
	for (GLuint textureMaterialID : texturesID) {
		glDeleteTextures(1, &textureMaterialID);
	}
	texturesID.clear(); 
}