#include <glew.h>
#include <iostream>
#include <SDL.h>
#include <SDL3/SDL_events.h>
#include "Scene3g.h"
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include "Texture.h"

Scene3g::Scene3g() : shader{ nullptr }, drawInWireMode{ false }
{
	Debug::Info("Created Scene3g: ", __FILE__, __LINE__);
}

Scene3g::~Scene3g() {
	Debug::Info("Deleted Scene3g: ", __FILE__, __LINE__);
}

bool Scene3g::OnCreate() {

	camera.getProjection().print("Cam projection");
	camera.getView().print("Cam view");

	sphere = new Body;
	sphere->OnCreate();

	sphereMesh = new Mesh("meshes/Cube.obj");
	sphereMesh->OnCreate();

	sphereTexture = new Texture();
	if (sphereTexture->LoadImage("textures/posy.jpg") == false) {
		std::cerr << "OH NO!\n";
		return false;
	}	
	skybox = new Skybox("textures/posx.jpg", "textures/negx.jpg",
		"textures/posy.jpg", "textures/negy.jpg",
		"textures/posz.jpg", "textures/negz.jpg");
	
	sphere->GetModelMatrix() = MMath::translate(0.0f, 0.0f, -2.0f);

	lightPos = Vec3(3.0f, 0.0f, 0.0f);

	shader = new Shader("shaders/reflectionVert.glsl", "shaders/reflectionFrag.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}
	
	return true;
}

void Scene3g::OnDestroy() {
	Debug::Info("Deleting assets Scene02g: ", __FILE__, __LINE__);

	shader->OnDestroy();
	delete shader;
}

void Scene3g::HandleEvents(const SDL_Event& sdlEvent) {
	camera.HandleEvents(sdlEvent);
	switch (sdlEvent.type) {
	case SDL_EVENT_KEY_DOWN:
		switch (sdlEvent.key.scancode) {
		case SDL_SCANCODE_W:
			drawInWireMode = !drawInWireMode;
			break;
		default:
			break;
		}
		break;
	}
}

void Scene3g::Update(const float deltaTime) {
	static float totalTime = 0.0f;
	totalTime += deltaTime;
}

void Scene3g::Render() const {
	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	/// This will draw the skybox
	camera.Render();

	glEnable(GL_DEPTH_TEST);
	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera.getProjection());
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, camera.getView());
	
	
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, sphere->GetModelMatrix());
	skybox->LoadImages();
	sphereMesh->Render();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sphereTexture->getTextureID());
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTextureID());



	
	glUseProgram(0);
}




