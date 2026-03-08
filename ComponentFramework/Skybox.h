#pragma once
#include <glew.h>
#include "Mesh.h"
#include "Shader.h"
#include <SDL3_image/SDL_image.h>

class Skybox {
	GLuint textureID;
	Mesh* cubeMesh;
	Shader* skyboxShader;
	const char* posXFilename, * negXFilename, * posYFilename, * negYFilename, * posZFilename, * negZFilename;

public: 
	Skybox(const char* posXFilename_, const char* negXFilename_, const char* posYFilename_, const char* negYFilename_, const char* posZFilename_, const char* negZFilename_, Mesh* cube_);
	~Skybox();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime);
	void Render() const;
	bool LoadImages();
	inline GLuint getTextureID() const { return textureID; }


};