#include "ModuleTexture.h"
//#include "DirectXTex.h"

ModuleTexture::ModuleTexture() {

}
ModuleTexture::~ModuleTexture() {

}

void ModuleTexture::Load(char texture_path) {
	//1. Load image data with external library into CPU

	//2. Create and load OpenGL texture into GPU

	//3. Add texture coordinates(UVs) into VBO

}

void ModuleTexture::Draw() {
	//1. Add vertex attribute for passing texture coordinates to our vertex shader and bind VBO data
	//2. Add texture sampler to our fragment shader and bind texture to it
}