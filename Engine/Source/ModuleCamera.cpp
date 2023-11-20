#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"

ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init() {

	auto screen_surface = App->GetWindow()->GetScreenSurface();
	SetAspectRatio(screen_surface->w, screen_surface->h);

	frustum.type = FrustumType::PerspectiveFrustum;

	SetFOV(90.0f);
	SetPlaneDistances(0.1f, 250.0f);
	SetPosition(float3(0.0f, 2.0f, 10.0f));

	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;

	/* float3x3 rotation = float3x3::identity;
	frustum.front = rotation.WorldZ();
	frustum.up = rotation.WorldY(); */

	LookAt(float3(0.0f, 0.0f, 0.0f));

	return true;
}

update_status  ModuleCamera::Update() {
	MovementController();

	return UPDATE_CONTINUE;
}

void ModuleCamera::SetFOV(float fov_deg) {
	horizontal_fov = math::DegToRad(fov_deg);
	SetVerticalFOV(horizontal_fov, aspect_ratio);
}

void ModuleCamera::SetAspectRatio(float screen_width, float screen_height) {
	aspect_ratio = screen_width / screen_height;
	SetVerticalFOV(horizontal_fov, aspect_ratio);
}

void ModuleCamera::SetPlaneDistances(const float near_dist, const float far_dist) {
	far_plane = far_dist;
	near_plane = near_dist;
	frustum.nearPlaneDistance = near_plane;
	frustum.farPlaneDistance = far_plane;

	distance_NearFar = frustum.farPlaneDistance - frustum.nearPlaneDistance;
}

void ModuleCamera::SetPosition(float3 pos) {
	position = pos;
	frustum.pos = position;
}


void  ModuleCamera::SetVerticalFOV(float horiz_fov, float asp_ratio) {
	frustum.verticalFov = 2.f * atanf(tanf(horiz_fov * 0.5f) / asp_ratio);
}
float4x4 ModuleCamera::GetViewMatrix() const {
	return float4x4(frustum.ViewMatrix());
}

float4x4 ModuleCamera::GetProjectionMatrix() const {
	float4x4 projectionGL = frustum.ProjectionMatrix();
	return float4x4(projectionGL);
}

float4x4 ModuleCamera::GetModel() const {
	float4x4 model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),
		float4x4::RotateZ(pi / 4.0f),
		float3(2.0f, 1.0f, 1.0f));

	return float4x4(model);
}

void ModuleCamera::Rotation() {
	//frustum.Transform
}

float3x3 ModuleCamera::LookAt(const float3& look_position) {

	float3 direction = look_position - position;
	float3x3 LookAtMatrix = float3x3::LookAt(frustum.front, direction.Normalized(), frustum.up, float3::unitY);
	frustum.front = LookAtMatrix.MulDir(frustum.front).Normalized();
	frustum.up = LookAtMatrix.MulDir(frustum.up).Normalized();

	return LookAtMatrix;
}

void  ModuleCamera::MovementController() {

	//float3 movmentspeed =	float3(5.0f,5.0f,5.0f); 
	float movmentspeed = 0.5f;

	if (App->GetInput()->GetKey(SDL_SCANCODE_W)) {
		position -= frustum.front * movmentspeed;
	}
	else if (App->GetInput()->GetKey(SDL_SCANCODE_S)) {
		position += frustum.front * movmentspeed;
	}
	else if (App->GetInput()->GetKey(SDL_SCANCODE_A)) {
		position -= frustum.WorldRight() * movmentspeed;
	}
	else if (App->GetInput()->GetKey(SDL_SCANCODE_D)) {
		position += frustum.WorldRight() * movmentspeed;
	}
	else if (App->GetInput()->GetKey(SDL_SCANCODE_A)) {
		position -= frustum.up * movmentspeed;
	}
	else if (App->GetInput()->GetKey(SDL_SCANCODE_D)) {
		position += frustum.up * movmentspeed;
	}
	SetPosition(position);

}
