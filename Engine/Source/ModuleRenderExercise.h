#pragma once

#include "Module.h"
#include "Globals.h"

class ModuleProgram; 


class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();
	bool Init();
	update_status Update();
	void DestroyVBO(unsigned vbo);
	bool CleanUp();

	// Forward declaration
	const char* LoadShaderSource(const char* shader_file_name); 
	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);
	unsigned CompileShader(unsigned type, const char* source);

	unsigned helloProgram;
	unsigned vbo;


};