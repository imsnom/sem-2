#include "Skybox.h"


Skybox::Skybox(const char* posXFilename_, const char* negXFilename_,
	const char* posYFilename_, const char* negYFilename_,
	const char* posZFilename_, const char* negZFilename_, Mesh* cubeMesh_) {

	posXFilename = posXFilename_;
	negXFilename = negXFilename_;

	posYFilename = posYFilename_;
	negYFilename = negYFilename_;

	posZFilename = posZFilename_;
	negZFilename = negZFilename_;
	cubeMesh = cubeMesh_;

}



bool Skybox::OnCreate() {
	cubeMesh = new Mesh("meshes/Sphere.obj");
	cubeMesh->OnCreate();

	skyboxShader = new Shader("shaders/skyboxVert.glsl", "shaders/skyboxFrag.glsl");
	return true;
}


bool Skybox::LoadImages() {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	SDL_Surface* textureSurface;
	int mode;

	textureSurface = IMG_Load(posXFilename);
	

	if (textureSurface = nullptr) {
			return false;
	}
	mode = SDL_BYTESPERPIXEL(textureSurface->format) == 4 ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_DestroySurface(textureSurface);


	textureSurface = IMG_Load(negXFilename);

	if (textureSurface = nullptr) {
		return false;
	}
	mode = SDL_BYTESPERPIXEL(textureSurface->format) == 4 ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_DestroySurface(textureSurface); textureSurface = IMG_Load(posXFilename);


	textureSurface = IMG_Load(posYFilename);

	if (textureSurface = nullptr) {
		return false;
	}
	mode = SDL_BYTESPERPIXEL(textureSurface->format) == 4 ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_DestroySurface(textureSurface); textureSurface = IMG_Load(posXFilename);


	textureSurface = IMG_Load(negYFilename);

	if (textureSurface = nullptr) {
		return false;
	}
	mode = SDL_BYTESPERPIXEL(textureSurface->format) == 4 ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_DestroySurface(textureSurface); textureSurface = IMG_Load(posXFilename);


	textureSurface = IMG_Load(posZFilename);

	if (textureSurface = nullptr) {
		return false;
	}
	mode = SDL_BYTESPERPIXEL(textureSurface->format) == 4 ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_DestroySurface(textureSurface); textureSurface = IMG_Load(posXFilename);


	textureSurface = IMG_Load(negZFilename);

	if (textureSurface = nullptr) {
		return false;
	}
	mode = SDL_BYTESPERPIXEL(textureSurface->format) == 4 ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_DestroySurface(textureSurface);


	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return true;



	

}