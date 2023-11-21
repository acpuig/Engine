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
	SetPosition(float3(0.0f, 0.0f, 10.0f));

	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;

	rotation_Matrix = float3x3::identity;

	view_Matrix = LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);

	return true;
}

update_status  ModuleCamera::Update() {
	MovementController(0.5);
	Zoom(0.5);
	return UPDATE_CONTINUE;
}

void ModuleCamera::SetFOV(float fov_deg) {
	horizontal_fov = math::DegToRad(fov_deg);
	SetVerticalFOV(horizontal_fov, aspect_ratio);
}

void ModuleCamera::SetAspectRatio(int screen_width, int screen_height) {
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

float4x4 ModuleCamera::LookAt(const float3& eye_position, const float3& target_position, const float3& up_position) {

	float3 forward = target_position - eye_position;
	float3 right = math::Cross(forward.Normalized(), up_position.Normalized());
	float3 up = math::Cross(right.Normalized(), forward.Normalized());
	position = eye_position;
	right = right.Normalized();
	forward = forward.Normalized();
	up = up.Normalized();

	float4x4 LookAtMatrix = float4x4({ right.x, up.x,-forward.x, position.x }, { right.y,up.y,-forward.y, position.y }, { right.z, up.z,-forward.z, position.z }, { 0,0,0,1 });
	return LookAtMatrix;
}

void ModuleCamera::RotateFrustum(char axis, float angle, const float delta_time) {
	float3 oldFront = frustum.front.Normalized();
	float3 oldUp = frustum.up.Normalized(); 

	switch (axis){
	case 'X': 		 // Rotate around the X-axis (pitch)
		frustum.front = rotation_Matrix.RotateX(angle * delta_time).Mul(oldFront);
		frustum.up = rotation_Matrix.RotateX(angle * delta_time).Mul(oldUp);
		break; 
	case 'Y': 		// Rotate around the Y-axis (yaw)
		frustum.front = rotation_Matrix.RotateY(angle * delta_time).Mul(oldFront);
		frustum.up = rotation_Matrix.RotateY(angle * delta_time).Mul(oldUp);
		break; 
	case 'Z':		// Rotate around the Z-axis (roll)
		frustum.front = rotation_Matrix.RotateZ(angle * delta_time).Mul(oldFront);
		frustum.up = rotation_Matrix.RotateZ(angle * delta_time).Mul(oldUp);
		break; 
	}
}



void  ModuleCamera::MovementController(const float delta_time) {

	float movment_speed = 0.5f;
	float rotation_speed = 0.5f;
	float zoom_speed = 0.5f;

	if (App->GetInput()->GetKey(SDL_SCANCODE_W)) {
		position += frustum.front * movment_speed * delta_time;
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_S)) {
		position -= frustum.front * movment_speed * delta_time;
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_A)) {
		position -= frustum.WorldRight() * movment_speed * delta_time;
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_D)) {
		position += frustum.WorldRight() * movment_speed * delta_time;
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_Q)) {
		position += frustum.up * movment_speed * delta_time;
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_E)) {
		position -= frustum.up * movment_speed * delta_time;
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_T)) { 
		// Rotate around the Y-axis (yaw)
		RotateFrustum('Y' , DegToRad(180), rotation_speed);
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_Y)) { 
		 // Rotate around the X-axis (pitch)
		RotateFrustum('X' , DegToRad(180), rotation_speed);
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_U)) {
		// Rotate around the Z-axis (roll)
		RotateFrustum('Z', DegToRad(180), rotation_speed);
	}
	SetPosition(position);
	view_Matrix = LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);
}


void ModuleCamera::Zoom(const float fov_diffdeg) {
	float minhoriz_fov = 1.0f; 
	float maxhoiz_fov = 2.5f;
	horizontal_fov = math::DegToRad(fov_diffdeg);
	if (horizontal_fov > maxhoiz_fov) {
		horizontal_fov = maxhoiz_fov;
	}
	if (horizontal_fov < minhoriz_fov) {
		horizontal_fov = minhoriz_fov;
	}

	int zoom_speed = 0.5;
	int scroll_value = App->GetInput()->GetMouse();
	//horizontal_fov -= zoom_speed * -scroll_value;
	
	//Scroll up
	if (scroll_value >  0) {
		horizontal_fov +=  fov_diffdeg * scroll_value;
	} 
	//Scroll down
	 if (scroll_value < 0) {
		horizontal_fov -= fov_diffdeg * scroll_value;
	}
	
	SetVerticalFOV(horizontal_fov, aspect_ratio);

}