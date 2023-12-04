#pragma once

#include <vector>
#include <string>
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float2.h"
#include "ModuleTexture.h"


namespace  tinygltf
{
	class Model;
	struct Mesh;
	struct Primitive;
}

class Mesh {
public:
	void Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void Render();
	void Draw(const std::vector<Texture>& textures);
	void Cleanup();


private:

	std::string name;

	unsigned  vbo;	//vertex buffer object
	unsigned ebo;   //element buffer object
	unsigned vao;	//vertex array object

	int vertexCountPos;  
	int vertexCountNormal;  
	int vertexCountTexCoord;  

	unsigned indexCount;
	int materialIndex = NULL;
	int numVertex;

	struct Vertex {
		float3 position;
		float3 normal;
		float2 texCoord;
	};

	void CreateVBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void CreateVAO();


};