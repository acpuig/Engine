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

	SetFOV(90.0f);
	SetPlaneDistances(0.1f, 100.0f);
	SetPosition(float3(0.0f, 0.0f, 0.0f));

	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);

	rotation_Matrix = float3x3::identity;

	// Set the initial position and orientation of the camera	
	float3 lookAtPoint = float3(0.0f, 0.0f, 0.0f);
	float3 cameraPosition = lookAtPoint +  float3(0.0f, 4.0f, 8.0f); // Adjust the height as needed
	float3 upVector = float3::unitY;
	float inclination_angle = math::DegToRad(180.0f);  // Inclination angle in degrees

	LookAt(cameraPosition, lookAtPoint, upVector, inclination_angle);
	//view_Matrix = LookAt(float3(0.0f, 4.0f, 8.0f), float3(0.0f, 0.0f, 0.0f), float3::unitY);
	return true;
}

update_status  ModuleCamera::Update() {
	const float rotation_speed = 0.50f;
	const float3 lookAtPoint = float3(0.0f, 0.0f, 0.0f);

	int dx = App->GetInput()->GetMousePosX();
	int dy = App->GetInput()->GetMousePosY();

	if ((App->GetInput()->GetMouseButtonRight() == true)) {
		//Orbit(dx, dy, rotation_speed, lookAtPoint);
	}

	MovementController(0.5);
	//Zoom(0.5);
	return UPDATE_CONTINUE;
}

void ModuleCamera::SetFOV( float fov_deg) {
	horizontal_fov = math::DegToRad(fov_deg);
	SetVerticalFOV(horizontal_fov, aspect_ratio);
}

void ModuleCamera::SetAspectRatio( int screen_width,  int screen_height) {
	aspect_ratio = float(screen_width / screen_height);
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
	 // Calculate the new forward, right, and up vectors
	 float3 forward = (target_position - eye_position).Normalized();
	 float3 right = up_position.Cross(forward).Normalized();

	 // Apply inclination rotation around the right axis
	 float3x3 inclination_rotation = float3x3::RotateAxisAngle(right, inclination_angle);
	 forward = inclination_rotation * forward;
	 up_position = inclination_rotation * up_position;
	 up_position = forward.Cross(right).Normalized();

	 // Set the new frustum orientation
	 frustum.front = -forward;
	 frustum.up = -up_position;

	 // Set the new position
	 SetPosition(eye_position);

	 // Update the view matrix
	 view_Matrix = float4x4({ right.x, up_position.x, -forward.x, eye_position.x }, { right.y, up_position.y, -forward.y, eye_position.y }, { right.z, up_position.z, -forward.z, eye_position.z }, { 0, 0, 0, 1 });

}

 // Function to update the camera position in an orbit around a target point
 void ModuleCamera::Orbit(int dx, int dy, float rotation_speed, const float3& lookAtPoint)
 {
	 
	// Calculate the rotation angles based on mouse movement
	float yaw = -dx * rotation_speed;
	float pitch = dy * rotation_speed;

	// Update the rotation matrix based on the calculated angles
	float3x3 yawRotation = float3x3::RotateAxisAngle(float3::unitY, math::DegToRad(yaw));
	float3x3 pitchRotation = float3x3::RotateAxisAngle(rotation_Matrix * float3::unitX, math::DegToRad(pitch));

	rotation_Matrix = pitchRotation * yawRotation;

	// Update the frustum orientation
	frustum.front = -(rotation_Matrix * float3::unitZ).Normalized();
	frustum.up = (rotation_Matrix * float3::unitY).Normalized();

	// Orbit around the lookAtPoint while maintaining the y-coordinate
	float3 orbitOffset = rotation_Matrix * float3(0.0f, 0.0f, orbit_radius);
	position = lookAtPoint + float3(orbitOffset.x, position.y, orbitOffset.z);
	frustum.pos = position;

	// Update the view matrix
	view_Matrix = frustum.ViewMatrix();
	 
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