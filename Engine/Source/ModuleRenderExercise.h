#pragma once

#include "Module.h"
#include "Globals.h"
#include <GL/glew.h>


class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();
	bool Init();
	update_status Update();
	update_status PostUpdate();

	void DestroyVBO(unsigned vbo);
	bool CleanUp();
	void RenderVBO(unsigned vbo, unsigned program);
	void RenderTriangle(unsigned vbo, unsigned program);

	void RenderQuad(unsigned textureID, unsigned program);

	// Forward declaration

	unsigned helloProgram;
	unsigned vbo;

private: 
	GLuint textureID;

};