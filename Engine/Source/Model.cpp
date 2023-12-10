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
#include "imgui/imgui.h"

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
			Mesh* mesh = new Mesh;
			mesh->Load(model, srcMesh, primitive);
			meshes.push_back(mesh);
		}
	}

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
			textureMaterialID = App->GetTexture()->Load(image.uri, wrap, min, mag);
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

void Model::MenuConfigTexture() {
	ImGui::Begin("Texture Loading Options");
	static int wrapValue = 2; // Default is GL_REPEAT
	static int mag_filter = 0; // Default is GL_LINEAR
	static int min_filter = 3; // Default is GL_NEAREST_MIPMAP_LINEAR

	const char* labels_wrap[] = { "Clamp to Border", "Clamp","Repeat", "Mirrored Repeat" };
	const unsigned values_wrap[] = { GL_CLAMP_TO_BORDER, GL_CLAMP, GL_REPEAT, GL_MIRRORED_REPEAT };
	const char* labels_mag[] = { "Linear", "Nearest" };
	const unsigned values_mag[] = { GL_LINEAR, GL_NEAREST };
	const char* labels_min[] = { "Nearest Mipmaps Nearest Criteria",
								"Nearest Mipmap Linear Criteria", "Linear Mipmaps (Two Closest) Nearest Criteria",
								"Linear Mipmaps (Two Closest) Linear Criteria" };
	const unsigned values_min[] = { GL_NEAREST_MIPMAP_NEAREST,
									GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR };

	ImGui::Combo("Wrap", &wrapValue, labels_wrap, IM_ARRAYSIZE(labels_wrap));
	ImGui::Combo("Mag Filter", &mag_filter, labels_mag, IM_ARRAYSIZE(labels_mag));
	ImGui::Combo("Min Filter", &min_filter, labels_min, IM_ARRAYSIZE(labels_min));

	ImGui::End();

	wrap = values_wrap[wrapValue];
	mag = values_mag[mag_filter];
	min = values_min[min_filter];

}

