#include "ModuleTexture.h"

ModuleTexture::ModuleTexture() {

}
ModuleTexture::~ModuleTexture() {

}

void ModuleTexture::Load(char texture_path, char texture_type) {
	//1. Load image data with external library into CPU
	/*switch (texture_type) {
	case ('dds' || 'DDS'):  DirectX::LoadFromDDSFile(texture_path,);

	case ('tga' || 'TGA'): 
		DirectX::ScratchImage::GetMetadata(); 

		DirectX::LoadFromTGAFile(texture_path, );
	default : //DirectX::LoadFromWICFile()
	}*/
	//2. Create and load OpenGL texture into GPU

	//3. Add texture coordinates(UVs) into VBO

}

void ModuleTexture::Draw() {
	//1. Add vertex attribute for passing texture coordinates to our vertex shader and bind VBO data
	//2. Add texture sampler to our fragment shader and bind texture to it
}