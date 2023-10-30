#include "Globals.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "ModuleProgram.h"
#include "ModuleOpenGL.h"
#include <GL/glew.h>



ModuleRenderExercise::ModuleRenderExercise()
{

}

// Destructor
ModuleRenderExercise::~ModuleRenderExercise()
{
}

bool ModuleRenderExercise::Init() {

	float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

	// Create a program with Hello World shaders
	const char* vertexShaderSource = App->GetProgram()->LoadShaderSource("default_vertex.glsl");
	const char* fragmentShaderSource = App->GetProgram()->LoadShaderSource("default_fragment.glsl");

	unsigned vertexShader = App->GetProgram()->CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned fragmentShader = App->GetProgram()->CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	 helloProgram = App->GetProgram()->CreateProgram(vertexShader, fragmentShader);

	 glDeleteShader(vertexShader);
	 glDeleteShader(fragmentShader);

	 return true;

}

update_status ModuleRenderExercise::Update()
{
	// Render the VBO triangle using Hello World program
	glUseProgram(helloProgram);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	App->GetOpenGL()->RenderVBO(vbo,helloProgram);

	return UPDATE_CONTINUE;
}

// This function must be called one time at destruction of vertex buffer
void ModuleRenderExercise::DestroyVBO(unsigned vbo)
{
	glDeleteBuffers(1, &vbo);
}

bool ModuleRenderExercise::CleanUp() 
{
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(helloProgram);
	return true;

}
