#pragma once
#include "Module.h"
#include "Globals.h"

#include "MathGeoLib.h"


class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera() ;

	bool Init() ;
	update_status Update() ;
	bool CleanUp() ;

	float4x4 GetView() ;
	float4x4 GetProjection() ;

private: 
	Frustum frustum;
};