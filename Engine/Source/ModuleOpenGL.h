#pragma once
#include "Module.h"

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

	void* GetContext() const {return context;}
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);

private:
	void* context;
	Model* cubeModel;
	HardwareDetection hardwareData;
};
