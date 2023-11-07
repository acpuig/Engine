#pragma once

#include "Module.h"
#include "Globals.h"


class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();
	bool Init();
	update_status Update();
	void DestroyVBO(unsigned vbo);
	bool CleanUp();
	void RenderVBO(unsigned vbo, unsigned program);

	// Forward declaration

	unsigned helloProgram;
	unsigned vbo;


};