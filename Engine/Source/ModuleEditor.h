#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleOpenGL.h"
#include "SDL.h"

struct InfoMenu {
	short devilVersion;
	HardwareDetection openglVersion;
	SDL_version sdlVersion;
	int CPUcount;
	float CPUcacheSize;
	float SystemRAM;
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
	Model* model;

};
