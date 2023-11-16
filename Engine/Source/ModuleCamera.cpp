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
	return float4x4(frustum.ViewMatrix());
}

float4x4 ModuleCamera::GetProjection() const
{
	return float4x4(frustum.ProjectionMatrix());
}

float4x4 ModuleCamera::GetModel() const
{
	float4x4 model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),
		float4x4::RotateZ(pi / 4.0f),

		float3(2.0f, 1.0f, 1.0f));

	return float4x4(model);


}

void ModuleCamera::InitFrustum() {

	frustum.type = FrustumType::PerspectiveFrustum;

	float3 position = float3(0, 0, 10); 
	frustum.pos = position;

	//frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f));
}

