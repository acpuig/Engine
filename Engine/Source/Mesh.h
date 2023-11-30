#pragma once
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "glew-2.1.0/include/GL/glew.h"#include <vector>
#include <string>
#include "tinygltf/tiny_gltf.h"

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
	void CreateVAO();
	void Render();
	void Draw();
	void Cleanup();


private:

	std::string name;
	std::vector<float> vertices;  // Vertex attributes (positions, normals, texture coordinates, etc.)
	std::vector<unsigned int> indices;  // Vertex indices
	unsigned program;

	unsigned vbo;	//vertex buffer object
	unsigned ebo;   //element buffer object
	unsigned vao;	//vertex array object

	GLsizei vertexCount;  // Add this member variable
	GLsizei indexCount;   // Assuming you have a member variable for index count too


};