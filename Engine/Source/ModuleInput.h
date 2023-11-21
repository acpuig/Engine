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
	const Uint32 GetMouse() const;


private:
	const Uint8 *keyboard = NULL;
	 Uint32 mouse = NULL;
};