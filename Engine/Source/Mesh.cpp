#include "Mesh.h"; 
#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "MathGeoLib/include/MathGeoLib.h"
#define TINYGLTF_IMPLEMENTATION

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE

#include "tinygltf/tiny_gltf.h"


// initialize a VBO
void Mesh::Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	CreateVBO(model, mesh, primitive);
	LoadEBO(model, mesh, primitive);
	CreateVAO();
}

void Mesh::CreateVBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive) 
{
	vertexCountPos = 0;
	vertexCountNormal = 0;
	const auto& itPos = primitive.attributes.find("POSITION");
	const auto& itNormal = primitive.attributes.find("NORMAL");
	//const auto& itTextCoord = primitive.attributes.find("TEXCOORD_0");

	if (itPos != primitive.attributes.end()) //Checking if Position Attribute Exists
	{
		//DIAPO 20 - Load all the vertex positions
		const tinygltf::Accessor& posAcc = model.accessors[itPos->second]; //Loading Vertex Positions
		const tinygltf::Accessor& normalAcc = model.accessors[itNormal->second];
		//const tinygltf::Accessor& textCoordAcc = model.accessors[itTextCoord->second];

		vertexCountPos += static_cast<GLsizei>(posAcc.count);
		vertexCountNormal += static_cast<GLsizei>(normalAcc.count);
		//vertexCountTexCoord += static_cast<GLsizei>(textCoordAcc.count);

		int numVertex = vertexCountPos + vertexCountNormal; 
		unsigned vertex_size = (sizeof(float3) + sizeof(float3) /*+ sizeof(float2)*/);
		unsigned buffer_size = vertex_size * numVertex ;

		assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		assert(posAcc.componentType == GL_FLOAT);
		assert(normalAcc.type == TINYGLTF_TYPE_VEC3);
		assert(normalAcc.componentType == GL_FLOAT);
		//assert(textCoordAcc.type == TINYGLTF_TYPE_VEC2);
		//assert(textCoordAcc.componentType == GL_FLOAT);

		// retrieve the actual position data about the buffer view and the buffer - 
		const tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
		const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);

		const tinygltf::BufferView& normalView = model.bufferViews[normalAcc.bufferView];
		const tinygltf::Buffer& normalBuffer = model.buffers[normalView.buffer];
		const unsigned char* bufferNormal = &(normalBuffer.data[normalAcc.byteOffset + normalView.byteOffset]);

		//const tinygltf::BufferView& texCoordView = model.bufferViews[textCoordAcc.bufferView];
		//const tinygltf::Buffer& texCoordBuffer = model.buffers[texCoordView.buffer];
		//const unsigned char* bufferTexCoord = &(texCoordBuffer.data[textCoordAcc.byteOffset + texCoordView.byteOffset]);

		//DIAPO 23 
		//Vertex* ptr = reinterpret_cast<Vertex*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)); //glMap -> glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * posAcc.count, nullptr, GL_STATIC_DRAW);
		Vertex* vertices = new Vertex[numVertex];
		//std::vector<Vertex> vertices[numVertex];

		//GLenum error = glGetError();
		//std::cerr << "OpenGL Error: " << error << std::endl;
		//if (error != GL_NO_ERROR) {
			//float3* ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
			for (size_t i = 0; i < posAcc.count; ++i)
			{
				//Iterates through each vertex, and position data is copied from the model's buffer to the OpenGL buffer.
				vertices[i].position = *reinterpret_cast<const float3*>(bufferPos);
				vertices[i].normal = *reinterpret_cast<const float3*>(bufferNormal);
				//ptr[i].texCoord = *reinterpret_cast<const float2*>(bufferTexCoord);

				bufferPos += sizeof(float3);
				bufferNormal += sizeof(float3);
				//bufferTexCoord += sizeof(float2);
			}
		//}
		//Mapping Buffer to OpenGL Buffer Object:
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertex, vertices, GL_STATIC_DRAW);

		glUnmapBuffer(GL_ARRAY_BUFFER);
		delete[] vertices;

	}
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

void Mesh::CreateVAO()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));
	/*glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (3 + 3))); // Texture coords*/
	glBindVertexArray(0);
}

void Mesh::Render( const std::vector<Texture>& textures) //Rendering with an EBO
{
	//Draw
	App->GetProgram()->UseProgram(); 
	App->GetProgram()->SendToShader("model", &App->GetCamera()->GetModel()[0][0]); 
	App->GetProgram()->SendToShader("view", &App->GetCamera()->GetViewMatrix()[0][0]);
	App->GetProgram()->SendToShader("proj", &App->GetCamera()->GetProjectionMatrix()[0][0]);

	// Bind texture
	//glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	if (!textures.empty() && materialIndex < textures.size()) {
		glBindTexture(GL_TEXTURE_2D, textures[materialIndex].id);
	}

	// Bind buffers and draw
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

	//glUniform1i(glGetUniformLocation(program, "diffuse"), 0);

//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

}

void Mesh::Cleanup()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Disable texture unit
	glDisable(GL_TEXTURE_2D);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(2);
	glDeleteBuffers(1, &vbo);
	
}


/*void Mesh::CreateVBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive) {

	const auto& itPos = primitive.attributes.find("POSITION");
	const auto& itNormal = primitive.attributes.find("NORMAL");
	//const auto& itTextCoord = primitive.attributes.find("TEXCOORD_0");

	if (itPos != primitive.attributes.end()) //Checking if Position Attribute Exists
	{
		//DIAPO 20 - Load all the vertex positions
		const tinygltf::Accessor& posAcc = model.accessors[itPos->second]; //Loading Vertex Positions
		const tinygltf::Accessor& normalAcc = model.accessors[itNormal->second];
		//const tinygltf::Accessor& textCoordAcc = model.accessors[itTextCoord->second];

		vertexCountPos += static_cast<GLsizei>(posAcc.count);
		vertexCountNormal += static_cast<GLsizei>(normalAcc.count);
		//vertexCountTexCoord += static_cast<GLsizei>(textCoordAcc.count);

		assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		assert(posAcc.componentType == GL_FLOAT);
		assert(normalAcc.type == TINYGLTF_TYPE_VEC3);
		assert(normalAcc.componentType == GL_FLOAT);
		//assert(textCoordAcc.type == TINYGLTF_TYPE_VEC2);
		//assert(textCoordAcc.componentType == GL_FLOAT);

		// retrieve the actual position data about the buffer view and the buffer -
		const tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
		const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);

		const tinygltf::BufferView& normalView = model.bufferViews[normalAcc.bufferView];
		const tinygltf::Buffer& normalBuffer = model.buffers[normalView.buffer];
		const unsigned char* bufferNormal = &(normalBuffer.data[normalAcc.byteOffset + normalView.byteOffset]);

		//const tinygltf::BufferView& texCoordView = model.bufferViews[textCoordAcc.bufferView];
		//const tinygltf::Buffer& texCoordBuffer = model.buffers[texCoordView.buffer];
		//const unsigned char* bufferTexCoord = &(texCoordBuffer.data[textCoordAcc.byteOffset + texCoordView.byteOffset]);

		//DIAPO 23
		//Mapping Buffer to OpenGL Buffer Object:
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * posAcc.count, nullptr, GL_STATIC_DRAW);

		Vertex* ptr = reinterpret_cast<Vertex*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)); //glMap -> glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * posAcc.count, nullptr, GL_STATIC_DRAW);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {

			//float3* ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
			for (size_t i = 0; i < posAcc.count; ++i)
			{
				//Iterates through each vertex, and position data is copied from the model's buffer to the OpenGL buffer.
				ptr[i].position = *reinterpret_cast<const float3*>(bufferPos);
				ptr[i].normal = *reinterpret_cast<const float3*>(bufferNormal);
				//ptr[i].texCoord = *reinterpret_cast<const float2*>(bufferTexCoord);

				bufferPos += sizeof(float3);
				bufferNormal += sizeof(float3);
				//bufferTexCoord += sizeof(float2);
			}
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);

	}
}*/
