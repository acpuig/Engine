#pragma once
#include "Module.h"
#include "Globals.h"
#include <GL/glew.h>

#include "DirectXTex/DirectXTex.h"

struct Texture {
	bool load = false ;
	GLuint id;
	const wchar_t* path;
	unsigned width;
	unsigned height;
	float* uvs;
};

class ModuleTexture : public Module {
public: 

	ModuleTexture(); 
	bool Init();
	GLuint Load(const wchar_t* texture_path, GLint wrapParam, GLint minParam, GLint magParam, bool mipmap);


private: 

	bool imageLoad = false;
	DirectX::ScratchImage imageData;
	DirectX::TexMetadata imageMetadata;

	void LoadImage(const wchar_t* texture_path);
	GLuint LoadTexture(GLint wrapParam, GLint minParam, GLint magParam, bool mipmap);
};