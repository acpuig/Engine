#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"

#include "SDL/include/SDL.h"
#include "imgui/imgui_impl_sdl2.h"
#include "MathGeoLib/include/Math/float2.h"

ModuleInput::ModuleInput() 
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::Update()
{
   
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                return UPDATE_STOP;
            case SDL_WINDOWEVENT:
                if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    App->GetWindow()->WindowNewSize();
                break;
            case SDL_MOUSEWHEEL:
                 mouse_scroll = sdlEvent.wheel.y;
                 break;
            case SDL_MOUSEMOTION:
                // Update the mouse delta values
                mouse_pos_x = sdlEvent.motion.xrel;
                mouse_pos_y = sdlEvent.motion.yrel;
                break; 

            case SDL_MOUSEBUTTONDOWN:
                if (sdlEvent.button.button == SDL_BUTTON_LEFT)
                    mouse_button_left = true;  // Left button pressed
                else if (sdlEvent.button.button == SDL_BUTTON_RIGHT)
                    mouse_button_right = true;  // Right button pressed
                break;

            case SDL_MOUSEBUTTONUP:
                if (sdlEvent.button.button == SDL_BUTTON_LEFT)
                    mouse_button_left = false;  // Left button released
                else if (sdlEvent.button.button == SDL_BUTTON_RIGHT)
                    mouse_button_right = false;  // Right button released
                break;

        }
        //Sending Input Events to ImGui
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
    }

    keyboard = SDL_GetKeyboardState(NULL);
    //mouse = SDL_GetMouseState(NULL,NULL);

    
    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
    
}

const unsigned ModuleInput::GetKey(SDL_Scancode key) const {
    return keyboard[key];
}

const int ModuleInput::GetMouseScroll() const {
    return int(mouse_scroll);
}

const int ModuleInput::GetMousePosX() const {
    return mouse_pos_x;
}
const int ModuleInput::GetMousePosY() const {
    return mouse_pos_y;
}

bool ModuleInput::GetMouseButtonRight() const {
    return mouse_button_right;
}

bool ModuleInput::GetMouseButtonLeft() const {
    return mouse_button_left;
}