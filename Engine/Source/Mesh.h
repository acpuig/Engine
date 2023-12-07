#pragma once

#include <string>
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float2.h"
#include "glew-2.1.0/include/GL/glew.h"


namespace  tinygltf
{
	class Model;
	struct Mesh;
	struct Primitive;
}

class Mesh {
public:
	void Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void Render(GLuint textureMaterialID);
	void CleanUp();


private:

	std::string name;

	unsigned vbo = NULL;; 	//vertex buffer object
	unsigned ebo = NULL;;   //element buffer object
	unsigned vao = NULL;;	//vertex array object

	int vertexCountPos = NULL;
	int vertexCountNormal = NULL;
	int vertexCountTexCoord = NULL;

	int indexCount = NULL;
	int materialIndex = NULL;
	int numVertex = NULL;

	struct Vertex {
		float3 position;
		float3 normal;
		float2 texCoord;
	};

	void CreateVBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void CreateVAO();


};