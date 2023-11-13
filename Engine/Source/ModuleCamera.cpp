#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"

ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

float4x4 ModuleCamera::GetView() const
{
	return float4x4(frustrum.ViewMatrix());
}

float4x4 ModuleCamera::GetProjection() const
{
	return float4x4(frustrum.ViewProjMatrix());
}

float4x4 ModuleCamera::GetModel() const
{
	float4x4 model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),
		float4x4::RotateZ(pi / 4.0f),
		float3(2.0f, 1.0f, 1.0f));

	return float4x4(model);


}

