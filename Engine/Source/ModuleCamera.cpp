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
	SetPlaneDistances(0.1f, 100.0f);
	SetPosition(float3(0.0f, 0.0f, 10.0f));

	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);

	rotation_Matrix = float3x3::identity;

	// Set the initial position and orientation of the camera
	float3 cameraPosition = float3(0.0f, 4.0f, 8.0f); // Adjust the height as needed
	float3 lookAtPoint = float3(0.0f, 0.0f, 0.0f);
	float3 upVector = float3::unitY;


	view_Matrix = LookAt(cameraPosition, lookAtPoint, upVector);
	//view_Matrix = LookAt(float3(0.0f, 4.0f, 8.0f), float3(0.0f, 0.0f, 0.0f), float3::unitY);
	return true;
}

update_status  ModuleCamera::Update() {
	const float rotation_speed = 1.0f;

	if (SDL_BUTTON(SDL_BUTTON_RIGHT)) {
		int dx = App->GetInput()->GetMousePosX();
		int dy = App->GetInput()->GetMousePosY();

		// Update the camera rotation based on mouse movement
		//RotateCameraWithMouse(dx, dy, rotation_speed);
	}
	

	MovementController(0.5);
	Zoom(0.5);
	return UPDATE_CONTINUE;
}

void ModuleCamera::SetFOV( float fov_deg) {
	horizontal_fov = math::DegToRad(fov_deg);
	SetVerticalFOV(horizontal_fov, aspect_ratio);
}

void ModuleCamera::SetAspectRatio( int screen_width,  int screen_height) {
	aspect_ratio = float(screen_width / screen_height);
	SetVerticalFOV(horizontal_fov, aspect_ratio);
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

 float4x4 ModuleCamera::GetModel()  {

	float4x4 model = float4x4::FromTRS(float3(0.0f, 0.0f, 0.0f),
		float4x4::RotateZ(0.0f),
		float3(40.0f, 40.0f, 40.0f));

	return float4x4(model);
}

 void ModuleCamera::RotateCameraWithMouse(int dx, int dy, float sensitivity) {
	 // Adjust these values based on your desired sensitivity
	 LOG("Mouse DeltaX: %d, DeltaY: %d\n", dx, dy);
	 const float maxRotationX = math::pi / 2.0f;  // Limit rotation around X-axis to avoid flipping

	 // Update the camera rotation matrix based on mouse movement
	 float3x3 rotation = float3x3::RotateY(-dx * sensitivity) * float3x3::RotateAxisAngle(frustum.WorldRight(), -dy * sensitivity);

	 // Apply the rotation to the camera's front, up, and right vectors
	 frustum.front = rotation * frustum.front;
	 frustum.up = rotation * frustum.up;
	 frustum.WorldRight() = rotation * frustum.WorldRight();

	 // Make sure the vectors are still orthogonal and normalized
	 frustum.front.Normalize();
	 frustum.up.Normalize();
	 frustum.WorldRight().Normalize();

	 // Limit rotation around X-axis to avoid flipping
	 float dotUpFront = frustum.up.Dot(frustum.front);
	 if (fabsf(dotUpFront) > math::eps) {
		 float angle = acosf(math::Clamp(dotUpFront, -1.0f, 1.0f));
		 if (angle > maxRotationX) {
			 float correctionAngle = angle - maxRotationX;
			 float3 rotationAxis = frustum.up.Cross(frustum.front).Normalized();
			 frustum.up = float3x3::RotateAxisAngle(rotationAxis, -correctionAngle) * frustum.up;
		 }
	 }
	 // Update the view matrix with the new orientation
	 view_Matrix = frustum.ViewMatrix();
 }

float4x4 ModuleCamera::LookAt(const float3& eye_position, const float3& target_position, const float3& up_position) {

	float3 forward = (target_position - eye_position).Normalized();
	float3 right = (up_position.Cross(forward)).Normalized();
	float3 up = forward.Cross(right).Normalized();
	SetPosition(eye_position);

	float4x4 LookAtMatrix = float4x4({ right.x, up.x, -forward.x, eye_position.x }, { right.y, up.y, -forward.y, eye_position.y }, { right.z, up.z, -forward.z, eye_position.z }, { 0, 0, 0, 1 });
	view_Matrix = LookAtMatrix; 
	return LookAtMatrix;
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



void  ModuleCamera::MovementController(const float delta_time) {

	float movment_speed = 0.2f;
	float rotation_speed = 0.2f;

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
	if (App->GetInput()->GetKey(SDL_SCANCODE_Y)) { 
		// Rotate around the Y-axis (yaw)
		RotateFrustum('Y' , DegToRad(90), rotation_speed);
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_X)) { 
		 // Rotate around the X-axis (pitch)
		RotateFrustum('X' , DegToRad(90), rotation_speed);
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_Z)) {
		// Rotate around the Z-axis (roll)
		RotateFrustum('Z', DegToRad(90), rotation_speed);
	}
	SetPosition(position);
	//view_Matrix = LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);
}


void ModuleCamera::Zoom(const float fov_diffdeg) {
	float minhoriz_fov = DegToRad(5); 
	float maxhoiz_fov = DegToRad(180);
	float angle = math::DegToRad(fov_diffdeg);

	horizontal_fov = frustum.horizontalFov; 

	if (horizontal_fov > maxhoiz_fov) {
		horizontal_fov = maxhoiz_fov;
	}
	if (horizontal_fov < minhoriz_fov) {
		horizontal_fov = minhoriz_fov;
	}

	float zoom_speed = 0.5;
	int scroll_value = App->GetInput()->GetMouseScroll();
	float aux = angle * zoom_speed * scroll_value;
	horizontal_fov += angle*  zoom_speed * scroll_value;
	
	//Scroll up
	SetVerticalFOV(horizontal_fov, aspect_ratio);

}