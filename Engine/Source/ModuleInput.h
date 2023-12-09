#pragma once
#include "Module.h"
#include "Globals.h"
#include <SDL_scancode.h>

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();

	const unsigned GetKey(SDL_Scancode key) const;
	const int GetMouseScroll() const;
	const bool GetScroll() const;
	const int GetMousePosX() const;
	const int GetMousePosY() const;
	bool GetMouseButtonRight() const;
	bool GetMouseButtonLeft() const;

	// Agrega esta función en ModuleInput.h
	void ResetMouseScroll();


private:
	const Uint8 *keyboard = NULL;
	 Sint32 mouse_scroll;
	 const Uint8 *mouse_button;
	 int mouse_pos_x;
	 int mouse_pos_y;
	 bool mouse_button_right = false;
	 bool mouse_button_left = false;
	 bool scroll;

};