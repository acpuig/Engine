#include "Module.h"

#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float3x3.h"
#include "MathGeoLib/include/Geometry/Frustum.h"

enum class ZoomLevel
{
    Close,
    Mid,
    Far
};

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();

	update_status Update();
	ZoomLevel currentZoomLevel = ZoomLevel::Mid;

	 float4x4 GetViewMatrix()  ;
	 float4x4 GetProjectionMatrix() ;
	 void SetModel(float3 scaleVector) ;
	// void RotateCameraWithMouse(int dx, int dy, float sensitivity);
	 float4x4 GetModel();
	 void WindowResized(unsigned int screen_width, unsigned int screen_height);
	 float3 ScreenToWorldPoint(int screen_x, int screen_y);

private:
	Frustum frustum;
	float near_plane;
	float far_plane;
	float delta_time = 0.5f;
	float zoomSpeed;

	float3x3 rotation_Matrix;
	float4x4 view_Matrix;
	float4x4 project_Matrix;
	float4x4 model;


	float aspect_ratio;
	float horizontal_fov;
	float3 position;
	float distance_NearFar;
	float orbit_radius = 8.0f;


	void SetAspectRatio(const int screen_width, const int screen_height);
	void SetFOV(const float fov_deg);
	void SetPlaneDistances(const float near_dist, const float far_dist);
	void SetPosition(float3 pos);
	void SetVerticalFOV(float horiz_fov, float asp_ratio);

	void RotateFrustum(char axis, float angle, const float delta_time);
	void Orbit(int dx, int dy, float rotation_speed, const float3& lookAtPoint);
	void MovementController(const float delta_time);

	void LookAt(const float3& eye_position, const float3& target_position,  float3& up_position, const float inclination_angle);
	void Zoom();
	void Reset();
};