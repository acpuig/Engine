
#include "ModuleTexture.h"
#include "DirectXTex/DirectXTex/DirectXTex.h"


ModuleTexture::ModuleTexture()
{
}

bool ModuleTexture::Init() {

	return true; 
}

GLuint ModuleTexture::Load(const wchar_t* path, GLint wrapParam, GLint minParam, GLint magParam, bool mipmap) {
	//1. Load image data with external library into CPU
	Texture texture;
	texture.path = path; 
	LoadImage(path);
	if (imageLoad) {
		//2. Create and load OpenGL texture into GPU
		texture.id = LoadTexture( wrapParam,  minParam,  magParam,  mipmap);

		//3. Add texture coordinates(UVs) into VBO
		texture.uvs = new float[imageMetadata.width * imageMetadata.height * 2];
		for (unsigned i = 0; i < imageMetadata.width * imageMetadata.height; ++i) {
			texture.uvs[2 * i] = static_cast<float>(i % imageMetadata.width) / static_cast<float>(imageMetadata.width - 1);
			texture.uvs[2 * i + 1] = static_cast<float>(i / imageMetadata.width) / static_cast<float>(imageMetadata.height - 1);
		}
	}
	//delete[] texture.uvs; // Add this line to avoid memory leaks

	return texture.id; 
}



void ModuleTexture::LoadImage(const wchar_t* image_path){

	// Try loading DDS
	if (DirectX::LoadFromDDSFile(image_path, DirectX::DDS_FLAGS_NONE, nullptr, imageData) == S_OK) {
		imageMetadata = imageData.GetMetadata();
		imageLoad = true;
		return;
	}

	// If loading DDS fails, try loading TGA
	if (DirectX::LoadFromTGAFile(image_path, nullptr, imageData) == S_OK) {
		imageMetadata = imageData.GetMetadata();
		imageLoad = true;
		return;
	}

	// If loading TGA also fails, try loading using WIC
	if (DirectX::LoadFromWICFile(image_path, DirectX::WIC_FLAGS_NONE, nullptr, imageData) == S_OK) {
		imageMetadata = imageData.GetMetadata();
		imageLoad = true;
	}
}

GLuint ModuleTexture::LoadTexture(GLint wrapParam, GLint minParam, GLint magParam, bool mipmap) {
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Set the texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magParam);

	// Assuming you have loaded the texture data into imageData

	GLenum internalFormat;
	GLenum format;
	GLenum type;

	switch (imageMetadata.format) {
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
	if(mipmap){
	//if (imageMetadata.mipLevels > 1) {
		// Assuming you have loaded mipmaps into imageData
		for (size_t i = 0; i < imageMetadata.mipLevels; ++i) {
			const DirectX::Image* mip = imageData.GetImage(i, 0, 0);
			glTexImage2D(GL_TEXTURE_2D, i, internalFormat, mip->width, mip->height, 0, format, type, mip->pixels);
		}

		// Generate mipmaps for the texture with multiple mip levels
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		// Assuming you have loaded the main texture into imageData
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imageMetadata.width, imageMetadata.height, 0, format, type, imageData.GetPixels());
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);

	return textureID;
}