
#include "Module.h"
#include "Globals.h"

#include "MathGeoLib.h"


class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera() ;

	bool Init();

	float4x4 GetViewMatrix() const;
	float4x4 GetProjectionMatrix() const;
	float4x4 GetModel() const;
	float3 GetPosition();



private:

	Frustum frustum;  
	float aspect_ratio;
	float horizontal_fov;
	float3 position;
	float distance_NearFar; 


	void SetAspectRatio(float screen_width, float screen_height);
	void SetPlaneDistances();
	void SetVerticalFOV();
	void SetHorizontalFOV();
	void SetPosition(const float3& new_position);
	void Orientation();
	void LookAt(int x, int y, int z);


};