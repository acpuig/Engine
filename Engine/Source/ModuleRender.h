#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	void RenderVBO(unsigned vbo, unsigned program);
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

};
