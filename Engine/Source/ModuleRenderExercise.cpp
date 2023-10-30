#include "Globals.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "ModuleRender.h"
#include "ModuleProgram.h"
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
	ModuleProgram ModuleProgram;
	const char* vertexShaderSource = ModuleProgram.LoadShaderSource("default_shader.glsl");
	const char* fragmentShaderSource = ModuleProgram.LoadShaderSource("default_fragment.glsl");

	unsigned vertexShader = ModuleProgram.CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned fragmentShader = ModuleProgram.CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	 helloProgram = ModuleProgram.CreateProgram(vertexShader, fragmentShader);

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
