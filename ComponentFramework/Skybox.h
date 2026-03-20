#pragma once
#include <glew.h>
#include "Mesh.h"
#include "Shader.h"
#include <SDL3_image/SDL_image.h>

class Skybox {
private:
	GLuint textureID;
	Mesh* cubeMesh;
	Shader* skyboxShader;
	const char* posXfilename, * negXfilename, * posYfilename, * negYfilename, * posZfilename, * negZfilename;
public:
	Skybox(const char* posXfilename_, const char* negXfilename_, const char* posYfilename_, const char* negYfilename_, const char* posZfilename_, const char* negZfilename_);
	~Skybox();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime);
	void Render() const;
	bool LoadImages();
	inline GLuint getTextureID() const { return textureID; }
	inline Shader* getSkyboxShader() {
		return skyboxShader;
	}

};

