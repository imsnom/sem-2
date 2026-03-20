#include "Skybox.h"


Skybox::Skybox(const char* posXfilename_, const char* negXfilename_,
	const char* posYfilename_, const char* negYfilename_,
	const char* posZfilename_, const char* negZfilename_) {
	posXfilename = posXfilename_;
	negXfilename = negXfilename_;

	posYfilename = posYfilename_;
	negYfilename = negYfilename_;

	posZfilename = posZfilename_;
	negZfilename = negZfilename_;
}

Skybox::~Skybox() {


}

void Skybox::Render() const {
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	cubeMesh->Render();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);


}

bool Skybox::OnCreate() {
	cubeMesh = new Mesh("meshes/Cube.obj");
	cubeMesh->OnCreate();

	skyboxShader = new Shader("shaders/skyboxVert.glsl", "shaders/skyboxFrag.glsl");
	skyboxShader->OnCreate();
	return true;
}


bool Skybox::LoadImages() {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	SDL_Surface* textureSurface;
	int mode;


	/******************************************************************************/
	textureSurface = IMG_Load(posXfilename);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (SDL_GetPixelFormatDetails(textureSurface->format)->bytes_per_pixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, mode, textureSurface->w,
		textureSurface->h, 0, mode, GL_UNSIGNED_BYTE,
		textureSurface->pixels);
	SDL_DestroySurface(textureSurface);
	/******************************************************************************/


	/******************************************************************************/
	textureSurface = IMG_Load(negXfilename);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (SDL_GetPixelFormatDetails(textureSurface->format)->bytes_per_pixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, mode, textureSurface->w,
		textureSurface->h, 0, mode, GL_UNSIGNED_BYTE,
		textureSurface->pixels);
	SDL_DestroySurface(textureSurface);


	textureSurface = IMG_Load(posYfilename);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (SDL_GetPixelFormatDetails(textureSurface->format)->bytes_per_pixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, mode, textureSurface->w,
		textureSurface->h, 0, mode, GL_UNSIGNED_BYTE,
		textureSurface->pixels);
	SDL_DestroySurface(textureSurface);



	textureSurface = IMG_Load(negYfilename);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (SDL_GetPixelFormatDetails(textureSurface->format)->bytes_per_pixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, mode, textureSurface->w,
		textureSurface->h, 0, mode, GL_UNSIGNED_BYTE,
		textureSurface->pixels);
	SDL_DestroySurface(textureSurface);
	/******************************************************************************/



	/******************************************************************************/
	textureSurface = IMG_Load(posZfilename);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (SDL_GetPixelFormatDetails(textureSurface->format)->bytes_per_pixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, mode, textureSurface->w,
		textureSurface->h, 0, mode, GL_UNSIGNED_BYTE,
		textureSurface->pixels);
	SDL_DestroySurface(textureSurface);
	/******************************************************************************/


	/******************************************************************************/
	textureSurface = IMG_Load(negZfilename);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (SDL_GetPixelFormatDetails(textureSurface->format)->bytes_per_pixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, mode, textureSurface->w,
		textureSurface->h, 0, mode, GL_UNSIGNED_BYTE,
		textureSurface->pixels);
	SDL_DestroySurface(textureSurface);
	/******************************************************************************/

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return true;

}