#include "Module.h"

#include "MathGeoLib/include/MathGeoLib.h"

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();

	update_status Update();

	 float4x4 GetViewMatrix()  ;
	 float4x4 GetProjectionMatrix() ;
	 float4x4 GetModel() ;


	//void Rotation();


private:
	Frustum frustum;
	float near_plane;
	float far_plane;

	float3x3 rotation_Matrix;
	float4x4 view_Matrix;
	float4x4 project_Matrix;


	float aspect_ratio;
	float horizontal_fov;
	float3 position;
	float distance_NearFar;


	void SetAspectRatio(const int screen_width, const int screen_height);
	void SetFOV(const float fov_deg);
	void SetPlaneDistances(const float near_dist, const float far_dist);
	void SetPosition(float3 pos);
	void SetVerticalFOV(float horiz_fov, float asp_ratio);

	void RotateFrustum(char axis, float angle, const float delta_time);
	void MovementController(const float delta_time);

	float4x4 LookAt(const float3& eye_position, const float3& target_position, const float3& up_position);
	void Zoom(const float fov_diffdeg);
};