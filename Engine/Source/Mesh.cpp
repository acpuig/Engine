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
	vertexCountTexCoord = 0;
	const auto& itPos = primitive.attributes.find("POSITION");
	const auto& itNormal = primitive.attributes.find("NORMAL");
	const auto& itTextCoord = primitive.attributes.find("TEXCOORD_0");

	const unsigned char* bufferPos = nullptr;
	const unsigned char* bufferNormal = nullptr;
	const unsigned char* bufferTexCoord = nullptr;

	if (itPos != primitive.attributes.end()) //Checking if Position Attribute Exists
	{
		//DIAPO 20 - Load all the vertex positions
		const tinygltf::Accessor& posAcc = model.accessors[itPos->second]; //Loading Vertex Positions

		vertexCountPos += static_cast<GLsizei>(posAcc.count);
		numVertex = vertexCountPos;
		unsigned vertex_size = (sizeof(float3) + sizeof(float3) + sizeof(float2));
		unsigned buffer_size = vertex_size * numVertex;

		assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		assert(posAcc.componentType == GL_FLOAT);


		// retrieve the actual position data about the buffer view and the buffer - 
		const tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
		const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
		bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);
	}
	if (itNormal != primitive.attributes.end()) //Checking if Normal Attribute Exists
	{
		const tinygltf::Accessor& normalAcc = model.accessors[itNormal->second];
		vertexCountNormal += static_cast<GLsizei>(normalAcc.count);

		assert(normalAcc.type == TINYGLTF_TYPE_VEC3);
		assert(normalAcc.componentType == GL_FLOAT);
		

		const tinygltf::BufferView& normalView = model.bufferViews[normalAcc.bufferView];
		const tinygltf::Buffer& normalBuffer = model.buffers[normalView.buffer];
		bufferNormal = &(normalBuffer.data[normalAcc.byteOffset + normalView.byteOffset]);

		
	}
	if (itTextCoord != primitive.attributes.end())
	{ //Checking if Tex Attribute Exists

		const tinygltf::Accessor& textCoordAcc = model.accessors[itTextCoord->second];
		vertexCountTexCoord += static_cast<GLsizei>(textCoordAcc.count);

		assert(textCoordAcc.type == TINYGLTF_TYPE_VEC2);
		assert(textCoordAcc.componentType == GL_FLOAT);

		const tinygltf::BufferView& texCoordView = model.bufferViews[textCoordAcc.bufferView];
		const tinygltf::Buffer& texCoordBuffer = model.buffers[texCoordView.buffer];
		bufferTexCoord = &(texCoordBuffer.data[textCoordAcc.byteOffset + texCoordView.byteOffset]);
	}

	 std::vector<Vertex> vertices; // Assuming Vertex is a structure that holds position, normal, and texCoord

	for (size_t i = 0; i < numVertex; ++i)
	{
		// Use the loop index to access different positions in the 'vertices' array
		Vertex vertex;
		vertex.position = *reinterpret_cast<const float3*>(bufferPos);
		vertex.normal = *reinterpret_cast<const float3*>(bufferNormal);
		vertex.texCoord = *reinterpret_cast<const float2*>(bufferTexCoord);

		 vertices.push_back(vertex);

		// Move the buffer pointers to the next set of data
		bufferPos += sizeof(float3);
		bufferNormal += sizeof(float3);
		bufferTexCoord += sizeof(float2);
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

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
		 if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT)
		 {
			 const uint16_t* bufferInd = reinterpret_cast<const uint16_t*>(buffer);
			 for (uint16_t i = 0; i < indAcc.count; ++i) ptr[i] = bufferInd[i];
		 }
		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE) {
			const uint8_t* bufferInd = reinterpret_cast<const uint8_t*>(buffer);
			for (uint8_t i = 0; i < indAcc.count; ++i) ptr[i] = bufferInd[i];
		}
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER); 
	}
}

void Mesh::CreateVAO()
{
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "Create VAO");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// Define layout in shader
	glEnableVertexAttribArray(0); //position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1); //normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float3)));
	glEnableVertexAttribArray(2); //Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float3) + sizeof(float3)));
	glPopDebugGroup();

}

void Mesh::Render(GLuint textureMaterialID) //Rendering with an EBO
{
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "RenderMesh");
	float4x4 model, view, proj;
	view = App->GetCamera()->GetViewMatrix();
	proj = App->GetCamera()->GetProjectionMatrix();
	model = App->GetCamera()->GetModel();

		//glUseProgram(program);
	App->GetProgram()->UseProgram();

	App->GetProgram()->SendToShaderMatrix4fv("model", &model[0][0]);
	App->GetProgram()->SendToShaderMatrix4fv("view", &view[0][0]);
	App->GetProgram()->SendToShaderMatrix4fv("proj", &proj[0][0]); 

	glEnable(GL_TEXTURE_2D);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureMaterialID);

	glBindVertexArray(vao);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		LOG("OpenGL Error before draw call: %d", error);
	}

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); // Unbind VAO
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
	glDisable(GL_TEXTURE_2D); // Disable texture unit

	glPopDebugGroup();
}

void Mesh::CleanUp()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Disable texture unit
	glDisable(GL_TEXTURE_2D);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}