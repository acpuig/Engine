#include "Globals.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "ModuleProgram.h"
#include "ModuleOpenGL.h"
#include "ModuleCamera.h"
#include "ModuleDebugDraw.h"
#include "ModuleTexture.h"

#include <GL/glew.h>



ModuleRenderExercise::ModuleRenderExercise()
{

}

// Destructor
ModuleRenderExercise::~ModuleRenderExercise()
{
}

bool ModuleRenderExercise::Init() {
	
	// Generate VBO and bind vertex data
	//float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	float buffer_data[] = {
		-1.0f, -1.0f, 0.0f, // ? v0 pos
		 1.0f, -1.0f, 0.0f, // ? v1 pos
		 0.0f, 1.0f, 0.0f, // ? v2 pos
		0.0f, 1.0f, // ? v0 texcoord
		 1.0f, 1.0f, // ? v1 texcoord
		0.5f, 0.0f // ? v2 texcoord
	};
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW);
	App->GetCamera()->Init();
	helloProgram = App->GetProgram()->Init("default_vertex.glsl", "default_fragment.glsl");
	textureID = App->GetTexture()->Load(L"Test-image-Baboon.ppm");

	 return true;
}

update_status ModuleRenderExercise::Update()
{
	//RenderTriangle(vbo, helloProgram);
	RenderQuad(textureID, helloProgram);


	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate()
{
	int w, h;
	SDL_GetWindowSize(App->GetWindow()->window, &w, &h);
	App->GetDebugDraw()->Draw(App->GetCamera()->GetViewMatrix(), App->GetCamera()->GetProjectionMatrix(), w, h);

	return update_status();
}

// This function must be called one time at destruction of vertex buffer
void ModuleRenderExercise::DestroyVBO(unsigned vbo)
{
	glDeleteBuffers(1, &vbo);
}

bool ModuleRenderExercise::CleanUp() 
{
	DestroyVBO(vbo);
	glDeleteProgram(helloProgram);
	glDeleteTextures(1, &textureID);

	return true;
}

// This function must be called each frame for drawing the triangle
void ModuleRenderExercise::RenderVBO(unsigned vbo, unsigned program)
{

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glUseProgram(program);
	// 1 triangle to draw = 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void ModuleRenderExercise::RenderTriangle(unsigned vbo, unsigned program)
{
	float4x4 model, view, proj;
	view = App->GetCamera()->GetViewMatrix();
	proj = App->GetCamera()->GetProjectionMatrix();
	model = App->GetCamera()->GetModel();

	glUseProgram(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &proj[0][0]);

	// TODO: bind buffer and vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// 1 triangle to draw = 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void ModuleRenderExercise::RenderQuad(unsigned textureID, unsigned program) {
	float4x4 model, view, proj;
	view = App->GetCamera()->GetViewMatrix();
	proj = App->GetCamera()->GetProjectionMatrix();
	model = App->GetCamera()->GetModel();

	glUseProgram(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &proj[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * 3));

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Draw the quad (assuming you have 6 vertices for two triangles)
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
