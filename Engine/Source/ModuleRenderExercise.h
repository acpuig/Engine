#pragma once

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"


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
	float4x4 GetView();
	float4x4 GetProjection();
	float4x4 GetModel();

private:
	Frustum frustum;

};