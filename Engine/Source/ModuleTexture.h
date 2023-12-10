#pragma once
#include "Module.h"
#include "Globals.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "DirectXTex/DirectXTex/DirectXTex.h"
#include <string>

struct Texture {
	bool load = false ;
	GLuint id;
	std::string path;
	unsigned width;
	unsigned height;
	float* uvs;
};

class ModuleTexture : public Module {
public: 

	ModuleTexture(); 
	bool Init();
	GLuint Load(const std::string& texture_path, GLint wrapParam, GLint minParam, GLint magParam);


private: 

	bool imageLoad = false;
	DirectX::ScratchImage imageData;
	DirectX::TexMetadata imageMetadata;

	void LoadImage(const wchar_t* texture_path);
	GLuint LoadTexture(GLint wrapParam, GLint minParam, GLint magParam);
};