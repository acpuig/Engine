#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"

ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init() {

	frustum.type = FrustumType::PerspectiveFrustum;

	float3 position = float3(0, 0, 10);
	frustum.pos = position;

	//frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	return true; 
}

float4x4 ModuleCamera::GetViewMatrix() const {
	return float4x4(frustum.ViewMatrix());
}

float4x4 ModuleCamera::GetProjectionMatrix() const {
	float4x4 projectionGL = frustum.ProjectionMatrix();
	//frustum.ProjectionMatrix().Transpose();
	return float4x4(projectionGL);
}

float4x4 ModuleCamera::GetModel() const {
	float4x4 model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),
		float4x4::RotateZ(pi / 4.0f),  
		float3(2.0f, 1.0f, 1.0f));

	return float4x4(model);
}

float3 ModuleCamera::GetPosition() {
	position = frustum.pos;
	return position;
}

void ModuleCamera::SetVerticalFOV() {
	frustum.verticalFov = 2.f * atanf(tanf(frustum.horizontalFov * 0.5f) * aspect_ratio);
}

void ModuleCamera::SetHorizontalFOV() {
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) / aspect_ratio);
}

void ModuleCamera:: SetAspectRatio(float screen_width, float screen_height) {
	aspect_ratio = screen_width / screen_height;
}
void ModuleCamera::SetPlaneDistances() {
	distance_NearFar = frustum.farPlaneDistance - frustum.nearPlaneDistance;

}
void ModuleCamera::SetPosition(const float3& new_position) {
	frustum.pos =  new_position;
	position = new_position;
}
void ModuleCamera::Orientation() {

}
void ModuleCamera::LookAt(int x,int  y, int z) {
	float xdistance = position.x - x;
	float ydistance = position.y - y;
	float zdistance = position.z - z;

	float3 dir = float3 (xdistance, ydistance, zdistance);

}
