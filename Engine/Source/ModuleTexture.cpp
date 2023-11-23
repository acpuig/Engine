#include "ModuleTexture.h"
#include <GL/glew.h>
#include "DirectXTex/DirectXTex.h"

/*
ModuleTexture::ModuleTexture()
{
}

bool ModuleTexture::Init() {

	return true; 
}
Texture ModuleTexture::Load(const wchar_t* path, char image_type) {
	//1. Load image data with external library into CPU
	Texture texture;
	texture.path = path; 
	LoadImage(path, image_type);
	if (imageLoad == true) {
		//2. Create and load OpenGL texture into GPU
		texture = LoadTexture(texture);
	}
	//3. Add texture coordinates(UVs) into VBO

	return texture; 
}


void ModuleTexture::LoadImage(const wchar_t* image_path, char image_type) {
	
	switch (image_type)
	{
	case ('DDS'): DirectX::LoadFromDDSFile(image_path, DirectX::DDS_FLAGS_NONE, nullptr, imageData);
		imageMetadata = imageData.GetMetadata();
		imageLoad == true;
		break;
	case ('TGA'):  DirectX::LoadFromTGAFile(image_path, nullptr, imageData);
		imageMetadata = imageData.GetMetadata();
		 imageLoad == true;
		break;
	default:  DirectX::LoadFromWICFile(image_path, DirectX::WIC_FLAGS_NONE, nullptr, imageData);
		imageMetadata = imageData.GetMetadata();
		 imageLoad == true;
	}
}

Texture ModuleTexture::LoadTexture(Texture texture) {
	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, imageMetadata.mipLevels - 1);

	GLenum internalFormat;
	GLenum format;
	GLenum type;

	switch (imageMetadata.format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_BGRA;
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		internalFormat = GL_RGB8;
		format = GL_BGR;
		type = GL_UNSIGNED_BYTE;
		break;
	default:
		assert(false && "Unsupported format");
	}
	for (size_t i = 0; i < imageMetadata.mipLevels; ++i)
	{
		const DirectX::Image* mip = imageData.GetImage(i, 0, 0);
		glTexImage2D(GL_TEXTURE_2D, i, internalFormat, mip->width, mip->height, 0, format, type, mip->pixels);
	}

	glTexImage2D(GL_TEXTURE_2D, imageMetadata.mipLevels, internalFormat, texture.width = imageMetadata.width,
		texture.id = imageMetadata.height, 0, format, type, imageData.GetPixels());

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glDeleteTextures(1, &texture.id);
	texture.load = true;
	return texture; 
}*/