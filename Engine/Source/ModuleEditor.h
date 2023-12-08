#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleOpenGL.h"
#include "SDL.h"

struct InfoMenu {
	short devilVersion;
	HardwareDetection openglVersion;
	//FPS graph
	//memory consumption
	//Hardware Detection
	SDL_version sdlVersion;
	int CPUcount;
	float CPUcacheSize;
	float SystemRAM;
	//CAPS (?)
	//GPU
	//BRAND
	//VRAM Budget
	//VRAM Usage
	//VRAM Available
	//VRAM Reserved

};

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();


	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();



private:
	InfoMenu info; 
	void* context;
	void RequestBrowser(const char* link);
	void MainMenu();
	void Draw();
	void ConfigWindow();
	bool quit = false;

};
