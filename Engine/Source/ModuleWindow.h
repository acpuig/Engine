#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();

	SDL_Surface* GetScreenSurface();

	void SetWindowBrightness(float brightness);

	void SetFullScreen(bool fullscreen);

	void WindowNewSize();

	void SetResizable(bool resizable);

	void SetBorderless(bool borderless);

	void SetFullDesktop();

	void MenuConfigWindow();

	void GetMonitorResolution();

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screenSurface = nullptr;
	float brightness = 1.0f;
	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fulldesktop = false;

	int width;
	int height; 

	int originalWidth;
	int originalHeight;

	int maxWidth;
	int maxHeight;

};

#endif // __ModuleWindow_H__