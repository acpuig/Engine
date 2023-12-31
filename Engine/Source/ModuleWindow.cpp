#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleOpenGL.h"

#include "imgui/imgui.h"


ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{

	fullscreen = FULLSCREEN;

	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		//GetMonitorResolution();

		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;

		if(FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}
		if (resizable)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}
		originalWidth = SCREEN_WIDTH;
		originalHeight = SCREEN_HEIGHT;
		width = (int)(maxWidth);
		height = (int)(maxHeight);
		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, originalWidth, originalHeight, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			SDL_UpdateWindowSurface(window);
			screenSurface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

SDL_Surface* ModuleWindow::GetScreenSurface() {
	return screenSurface;
}

//Brightness
void ModuleWindow::SetWindowBrightness(float brightness)
{
	SDL_SetWindowBrightness(window, brightness);
}


//Fullscreen
void ModuleWindow::SetFullScreen(bool fullscreen)
{
	if (fullscreen){
		SDL_GetWindowSize(window, &originalWidth, &originalHeight);
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}else{
		SDL_SetWindowFullscreen(window, 0);
		SDL_SetWindowSize(window, originalWidth, originalHeight);
		App->GetCamera()->WindowResized(originalWidth,originalHeight);
	}
	SDL_UpdateWindowSurface(window);
}

void ModuleWindow::WindowNewSize() 
{
	SDL_UpdateWindowSurface(window);
	screenSurface = SDL_GetWindowSurface(window);
	App->GetOpenGL()->WindowResized(screenSurface->w, screenSurface->h);
	App->GetCamera()->WindowResized(screenSurface->w, screenSurface->h);
}

//Resizable
void ModuleWindow::SetResizable(bool resizable)
{
	SDL_SetWindowResizable(window, (SDL_bool)resizable);
}
//Borderless
void ModuleWindow::SetBorderless(bool borderless)
{
	SDL_SetWindowBordered(window, (SDL_bool)(!borderless)); 
}
//Full desktop
void ModuleWindow::SetFullDesktop()
{
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void ModuleWindow::MenuConfigWindow() {

	SDL_DisplayMode mode;
	SDL_GetDisplayMode(0, 0, &mode);
	int refresh_rate = mode.refresh_rate;

	ImGui::Text("Refresh Rate %lu", refresh_rate);
	if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f)) {
		SetWindowBrightness(brightness);
	} 
	if (fullscreen == false) {
		if (ImGui::Checkbox("Full Screen", &fullscreen)) {
			SetFullScreen(fullscreen);
		} 
		/*if (ImGui::Checkbox("Resizable", &resizable)) {
			SetResizable(resizable);
		}*/
		ImGui::SameLine();
		if (ImGui::Checkbox("Borderless", &borderless)) {
			SetBorderless(borderless);
		}
	}
	ImGui::Separator();
}

void ModuleWindow::GetMonitorResolution()
{
	RECT monitor;
	const HWND desktop_handle = GetDesktopWindow();
	GetWindowRect(desktop_handle, &monitor);
	maxWidth = monitor.right;
	maxHeight = monitor.bottom;
}

