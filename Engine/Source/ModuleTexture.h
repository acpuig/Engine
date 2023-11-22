#pragma once
#include "Module.h"
#include "Globals.h"
#include "DirectXTex/DirectXTex.h"


struct Texture {
	bool load = false ;
	unsigned id;
	const wchar_t* path;
	unsigned width;
	unsigned height;
};


class ModuleTexture : public Module {
public: 

	ModuleTexture(); 

	bool Init();
	Texture Load(const wchar_t* texture_path, char texture_type);
	void Draw();


	bool imageLoad = false;
	DirectX::ScratchImage& imageData;
	DirectX::TexMetadata imageMetadata;


private: 
	void LoadImage(const wchar_t* texture_path, char texture_type);
	Texture LoadTexture(Texture texture);
};