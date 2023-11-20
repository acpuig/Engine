#include "Module.h"
#include "Globals.h"

#include "MathGeoLib.h"


class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();

	update_status Update();

	float4x4 GetViewMatrix() const;
	float4x4 GetProjectionMatrix() const;
	float4x4 GetModel() const;

	void Rotation();


private:

	Frustum frustum;
	float near_plane;
	float far_plane;

	float aspect_ratio;
	float horizontal_fov;
	float3 position;
	float distance_NearFar;


	void SetAspectRatio(float screen_width, float screen_height);
	void SetFOV(float fov_deg);
	void MovementController();
	void SetPlaneDistances(const float near_dist, const float far_dist);
	void SetPosition(float3 pos);
	void SetVerticalFOV(float horiz_fov, float asp_ratio);

	float3x3 LookAt(const float3& look_position);

};