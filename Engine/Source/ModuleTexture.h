#pragma once
#include "Module.h"
#include "Globals.h"



class ModuleTexture : public Module {

public: 
	ModuleTexture();
	~ModuleTexture();

	void Load(char texture_path);
	void Draw();

private: 
	
};