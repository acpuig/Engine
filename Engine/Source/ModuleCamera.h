
#include "Module.h"
#include "Globals.h"

#include "MathGeoLib.h"


class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera() ;

	float4x4 GetView() const;
	float4x4 GetProjection() const;

	float4x4 GetModel() const;

private:

	Frustum frustrum;  

};