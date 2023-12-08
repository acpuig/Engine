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
		GetMonitorResolution();


		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;

		if(FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}
		if (resizable)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		//width = (int)(maxWidth);
		//height = (int)(maxHeight);
		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
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
	return screen_surface;
}

//Brightness
void ModuleWindow::SetWindowBrightness(float brightness)
{
	SDL_SetWindowBrightness(window, brightness);
}

//width
//height 
//Fullscreen
void ModuleWindow::SetFullScreen(bool fullscreen)
{
	if (fullscreen)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else
		SDL_SetWindowFullscreen(window, 0);
	SDL_UpdateWindowSurface(window);

}

void ModuleWindow::WindowNewSize() 
{
	SDL_UpdateWindowSurface(window);
	screen_surface = SDL_GetWindowSurface(window);
	App->GetOpenGL()->WindowResized(screen_surface->w, screen_surface->h);
	App->GetCamera()->WindowResized(screen_surface->w, screen_surface->h);
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
	if (ImGui::Checkbox("Full Screen", &fullscreen)) {
		SetFullScreen(fullscreen);
		ImGui::SameLine();

	}else if (ImGui::Checkbox("Resizable", &resizable)) {
		SetResizable(resizable);

	}else if (ImGui::Checkbox("Borderless", &borderless)) {
		SetBorderless(borderless);
		ImGui::SameLine();

	}else if (ImGui::Checkbox("Full Desktop", &fulldesktop)) {
		SetFullDesktop();
	}

	ImGui::Separator();
}

void ModuleWindow::GetMonitorResolution()
{
	RECT monitor;
	const HWND desktop_handle = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(desktop_handle, &monitor);
	maxWidth = monitor.right;
	maxHeight = monitor.bottom;
}

