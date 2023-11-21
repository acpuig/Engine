#pragma once
#include "Module.h"
#include "Globals.h"
#include "DirectXTex/DirectXTex.h"



class ModuleTexture : public Module {

public: 
	ModuleTexture();
	~ModuleTexture();

	void Load(char texture_path, char texture_type);

	void Draw();

private: 
	

};