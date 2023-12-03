#pragma once

#include <vector>
#include <string>
#include <Math/float3.h>
#include <Math/float2.h>
#include "ModuleTexture.h"


namespace  tinygltf
{
	class Model;
	struct Mesh;
	struct Primitive;
}
/*
struct Primitive {
	std::map<std::string, int> attributes; // attributes data(positions, normals, texture coordinates)
	int material{ -1 }; // material index attached to it
	int indices{ -1 }; // the vertex indices,

};
*/

class Mesh {
public:
	void Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void Render(float4x4& model, Texture texture);
	void Cleanup();


private:

	std::string name;
	std::vector<unsigned int> indices;  // Vertex indices

	unsigned  vbo;	//vertex buffer object
	unsigned ebo;   //element buffer object
	unsigned vao;	//vertex array object

	int vertexCountPos;  
	int vertexCountNormal;  
	int vertexCountTexCoord;  

	int indexCount;   // Assuming you have a member variable for index count too

	struct Vertex {
		float3 position;
		float3 normal;
		//float2 texCoord;
	};

	void CreateVBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void CreateVAO();


};