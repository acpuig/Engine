#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "SDL.h"

#include <GL/glew.h>

ModuleRender::ModuleRender()
{

}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Enable cull backward faces
	glEnable(GL_CULL_FACE);

	// Define front faces as counter-clockwise (this is the default)
	glFrontFace(GL_CCW);

	return true; 
}

// This function must be called each frame for drawing the triangle
void ModuleRender::RenderVBO(unsigned vbo, unsigned program)
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

update_status ModuleRender::PreUpdate()
{
	

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{

	return true;
}
