#include "Mesh.h"; 
#include "Globals.h"
#include "Application.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "SDL.h"
#include "MathGeoLib/include/MathGeoLib.h"
#define TINYGLTF_IMPLEMENTATION

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE

#include "tinygltf/tiny_gltf.h"


// initialize a VBO
void Mesh::Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{

	const auto& itPos = primitive.attributes.find("POSITION");
	if (itPos != primitive.attributes.end())
	{
		//DIAPO 20 - Load all the vertex positions
		const tinygltf::Accessor& posAcc = model.accessors[itPos->second];
		vertexCount = static_cast<GLsizei>(posAcc.count);

		SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(posAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
		const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);

		//DIAPO 23 
		glGenBuffers(1, &vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * posAcc.count, nullptr, GL_STATIC_DRAW);
		float3* ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		for (size_t i = 0; i < posAcc.count; ++i)
		{
			ptr[i] = *reinterpret_cast<const float3*>(bufferPos);
			bufferPos += posView.byteStride;
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	//we will want to load more attributes such as normals or tangents DIAPO 24
	const auto& itNormal = primitive.attributes.find("NORMAL");
	if (itNormal != primitive.attributes.end())
	{
		//DIAPO 20 - Load all the vertex positions
		const tinygltf::Accessor& normalAcc = model.accessors[itNormal->second];
		vertexCount = static_cast<GLsizei>(normalAcc.count);

		SDL_assert(normalAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(normalAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& normalView = model.bufferViews[normalAcc.bufferView];
		const tinygltf::Buffer& normalBuffer = model.buffers[normalView.buffer];
		const unsigned char* bufferNormal = &(normalBuffer.data[normalAcc.byteOffset + normalView.byteOffset]);

		//DIAPO 23 
		glGenBuffers(1, &vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * normalAcc.count, nullptr, GL_STATIC_DRAW);
		float3* ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		for (size_t i = 0; i < normalAcc.count; ++i)
		{
			ptr[i] = *reinterpret_cast<const float3*>(bufferNormal);
			bufferNormal += normalView.byteStride;
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	const auto& itTang = primitive.attributes.find("TANGENT");
	if (itTang != primitive.attributes.end())
	{
		//DIAPO 20 - Load all the vertex positions
		const tinygltf::Accessor& tangAcc = model.accessors[itTang->second];
		vertexCount = static_cast<GLsizei>(tangAcc.count);

		SDL_assert(tangAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(tangAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& tangView = model.bufferViews[tangAcc.bufferView];
		const tinygltf::Buffer& tangBuffer = model.buffers[tangView.buffer];
		const unsigned char* bufferTang = &(tangBuffer.data[tangAcc.byteOffset + tangView.byteOffset]);

		//DIAPO 23 
		glGenBuffers(1, &vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * tangAcc.count, nullptr, GL_STATIC_DRAW);
		float3* ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		for (size_t i = 0; i < tangAcc.count; ++i)
		{
			ptr[i] = *reinterpret_cast<const float3*>(bufferTang);
			bufferTang += tangView.byteStride;
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	//we want to load positions and texture coordinates
	

}

//Rendering separated arrays
void Mesh::LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	
	if (primitive.indices >= 0)
	{
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		const tinygltf::Accessor& indAcc = model.accessors[primitive.indices];
		indexCount = static_cast<GLsizei>(indAcc.count);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indAcc.count, nullptr, GL_STATIC_DRAW);
		unsigned int* ptr = reinterpret_cast<unsigned int*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));

		const tinygltf::BufferView& indView = model.bufferViews[indAcc.bufferView];
		const unsigned char* buffer = &(model.buffers[indView.buffer].data[indAcc.byteOffset +
			indView.byteOffset]);

		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT)
		{
			const uint32_t* bufferInd = reinterpret_cast<const uint32_t*>(buffer);
			for (uint32_t i = 0; i < indAcc.count; ++i) ptr[i] = bufferInd[i];
		}
		indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT;
		// TODO indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER); 
	}
}

void Mesh::Render() //Rendering with an EBO
{
		
	glUseProgram(program);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * vertexCount));

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	
}

void Mesh::CreateVAO()
{
	glGenVertexArrays(1, &vao); 

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * vertexCount));

	glBindVertexArray(0);
}

void Mesh::Draw()
{
	glUseProgram(program);
	//drawing a mesh
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, textures[materialIndex]);
	glUniform1i(glGetUniformLocation(program, "diffuse"), 0);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}

void Mesh::Cleanup()
{
	if (vbo != 0)
	{
		glDeleteBuffers(1, &vbo);
		vbo = 0;
	}

	if (ebo != 0)
	{
		glDeleteBuffers(1, &ebo);
		ebo = 0;
	}

	if (vao != 0)
	{
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}
}