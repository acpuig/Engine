#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "MathGeoLib/include/MathGeoLib.h"

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

	SetPlaneDistances(0.1f, 100.0f);
	SetPosition(float3(0.0f, 0.0f, 0.0f));

	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);

	rotation_Matrix = float3x3::identity;

	// Set the initial position and orientation of the camera	
	float3 lookAtPoint = float3(0.0f, 0.0f, 0.0f);
	float3 cameraPosition = lookAtPoint +  float3(0.0f, 4.0f, 8.0f);
	float3 upVector = float3::unitY;
	float inclination_angle = math::DegToRad(180.0f); 

	LookAt(cameraPosition, lookAtPoint, upVector, inclination_angle);
	return true;
}

void ModuleCamera::Reset() {

	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);

	rotation_Matrix = float3x3::identity;

	// Set the initial position and orientation of the camera	
	float3 lookAtPoint = float3(0.0f, 0.0f, 0.0f);
	float3 upVector = float3::unitY;
	float inclination_angle = math::DegToRad(180.0f);  

	LookAt(frustum.pos, lookAtPoint, upVector, inclination_angle);
}

update_status ModuleCamera::Update() {
	const float rotation_speed = 0.50f;
	const float3 lookAtPoint = float3(0.0f, 0.0f, 0.0f);

	int dx = App->GetInput()->GetMousePosX();
	int dy = App->GetInput()->GetMousePosY();

	MovementController(0.5);
	Zoom();
	return UPDATE_CONTINUE;
}

void ModuleCamera::SetFOV( float fov_deg) {
	horizontal_fov = math::DegToRad(fov_deg);
	SetVerticalFOV(horizontal_fov, aspect_ratio);
}

void ModuleCamera::SetAspectRatio( int screen_width,  int screen_height) {
	aspect_ratio = static_cast<float>(screen_width) / static_cast<float>(screen_height);
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);
}

void ModuleCamera::WindowResized(unsigned int screen_width, unsigned int screen_height)
{
	SetAspectRatio(screen_width, screen_height);
}

void  ModuleCamera::SetVerticalFOV(float horiz_fov, float asp_ratio) {
	frustum.verticalFov = 2.f * atanf(tanf(horiz_fov * 0.5f) / asp_ratio);
}

void ModuleCamera::SetPlaneDistances( float near_dist,  float far_dist) {
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

float4x4 ModuleCamera::GetViewMatrix()  {
	view_Matrix = (frustum.ViewMatrix());
	return float4x4(view_Matrix);
}

float4x4 ModuleCamera::GetProjectionMatrix()  {
	 project_Matrix = frustum.ProjectionMatrix();
	return float4x4(project_Matrix);
}

void ModuleCamera::SetModel(float3 scaleVector)  {
	 model = float4x4::FromTRS(float3(0.0f, 0.0f, 0.0f),
	float4x4::RotateZ(0.0f), scaleVector); 
}

float4x4 ModuleCamera::GetModel() {
	 return float4x4(model);
}

void ModuleCamera::LookAt(const float3& eye_position, const float3& target_position, float3& up_position, const float inclination_angle) {
	 float3 forward = (target_position - eye_position).Normalized();
	 float3 right = up_position.Cross(forward).Normalized();

	 // Apply inclination rotation around the right axis
	 float3x3 inclination_rotation = float3x3::RotateAxisAngle(right, inclination_angle);
	 forward = inclination_rotation * forward;
	 up_position = inclination_rotation * up_position;
	 up_position = forward.Cross(right).Normalized();

	 frustum.front = -forward;
	 frustum.up = -up_position;

	 SetPosition(eye_position);

	 view_Matrix = float4x4({ right.x, up_position.x, -forward.x, eye_position.x }, { right.y, up_position.y, -forward.y, eye_position.y }, { right.z, up_position.z, -forward.z, eye_position.z }, { 0, 0, 0, 1 });

}

void  ModuleCamera::MovementController(const float delta_time) {

	float movment_speed = 0.2f;
	float rotation_speed = 0.0002f;

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
	if (App->GetInput()->GetKey(SDL_SCANCODE_F)) {
		float3  lookAtPoint = float3(0.0f, 0.0f, 0.0f);
		float3 upVector = float3::unitY;
		float inclination_angle = math::DegToRad(180.0f);  
		LookAt(frustum.pos, lookAtPoint, upVector, inclination_angle);
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_R)) {
		Reset();
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_RIGHT)) { 
		// Rotate around the Y-axis (yaw)
		RotateFrustum('Y' , DegToRad(5), rotation_speed);
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_LEFT)) {
		// Rotate around the Y-axis (yaw)
		RotateFrustum('Y', -DegToRad(5), rotation_speed);
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_X)) {
		// Rotate around the X-axis (pitch)
		RotateFrustum('X', DegToRad(5), rotation_speed);
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_Z)) {
		// Rotate around the Z-axis (roll)
		RotateFrustum('Z', DegToRad(5), rotation_speed);
	}
	SetPosition(position);
}

void ModuleCamera::Zoom() {
	float minHorizFov = DegToRad(20);
	float maxHorizFov = DegToRad(90);
	const float zoom_speed = 0.1f;  

	int scrollValue = App->GetInput()->GetMouseScroll();

	if (scrollValue != 0)
	{
		float zoom_factor = 1.0f + scrollValue * zoom_speed;

		frustum.horizontalFov /= zoom_factor;
		frustum.verticalFov /= zoom_factor;

		project_Matrix = frustum.ProjectionMatrix();
		App->GetInput()->ResetMouseScroll();
	}
}


void ModuleCamera::RotateFrustum(char axis, float angle, const float delta_time) {
	float3 oldFront = frustum.front.Normalized();
	float3 oldUp = frustum.up.Normalized();
	float3 oldRight = frustum.WorldRight().Normalized();
	float3x3 rotation_Matrix;

	switch (axis) {
	case 'X': // Rotate around the X-axis (pitch)
		rotation_Matrix = float3x3::RotateAxisAngle(float3(1.0f, 0.0f, 0.0f), -angle); // Invert the angle
		frustum.front = rotation_Matrix.MulDir(oldFront);
		frustum.up = rotation_Matrix.MulDir(oldUp);
		break;
	case 'Y': // Rotate around the Y-axis (yaw)
		rotation_Matrix = float3x3::RotateY(-angle); // Invert the angle
		frustum.front = rotation_Matrix.MulDir(oldFront);
		frustum.up = rotation_Matrix.MulDir(oldUp);
		break;
	case 'Z': // Rotate around the Z-axis (roll)
		rotation_Matrix = float3x3::RotateAxisAngle(float3(0.0f, 0.0f, 1.0f), -angle); // Invert the angle
		frustum.WorldRight() = rotation_Matrix.MulDir(oldRight);
		frustum.up = rotation_Matrix.MulDir(oldUp);
		break;
	}
}