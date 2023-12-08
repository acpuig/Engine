#pragma once
#include "Module.h"
#include "MathGeoLib/include/Math/float4.h"
#include <vector>

class Model;
struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

struct HardwareDetection {
	unsigned char* glew;
	unsigned char* opengl;
	unsigned char* glsl;

	unsigned char* gpu;
	unsigned char* brand;
};

class ModuleOpenGL : public Module
{
public:

	ModuleOpenGL();
	~ModuleOpenGL();

	bool Init();
	HardwareDetection GetHardwareData();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();



	float modelScale = 1.0f;

	void* GetContext() const {return context;}
	bool CleanUp();
	void MenuConfigWindow();
	void MenuConfigModels();
	void MenuConfigApp();
	void WindowResized(int width, int height);

	std::vector<float> fps_log;
	std::vector<float> ms_log;

private:
	void* context;
	Model* model;
	HardwareDetection hardwareData;
	bool cubeModel = false;
	bool bakerModel = true;

	void Histogram();

	float4 clearColor;

};
