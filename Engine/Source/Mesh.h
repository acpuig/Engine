#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>
#include "tiny_gltf.h"

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
	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void RenderEBO();

	void CreateVAO();
	void DrawVAO();


private:

	std::string name;
	std::vector<float> vertices;  // Vertex attributes (positions, normals, texture coordinates, etc.)
	std::vector<unsigned int> indices;  // Vertex indices
};