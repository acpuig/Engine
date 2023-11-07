#include "ModuleCamera.h"

#include "Math/MathConstants.h"

#include <GL/glew.h>


ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	return false;
}

update_status ModuleCamera::Update()
{
	return update_status();
}

bool ModuleCamera::CleanUp()
{
	return false;
}

float4x4 ModuleCamera::GetView()
{
	float4x4 viewGL = float4x4(frustum.ViewMatrix()).Transposed();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*viewGL.v);

	return viewGL;
}

float4x4 ModuleCamera::GetProjection()
{
	float4x4 projectionGL = frustum.ProjectionMatrix().Transposed(); //<-- Important to transpose!
	//Send the frustum projection matrix to OpenGL
	// direct mode would be:
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*projectionGL.v);
	return projectionGL;
}
