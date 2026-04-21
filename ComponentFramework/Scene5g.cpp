#include <glew.h>
#include <iostream>
#include <SDL.h>
#include <SDL3/SDL_events.h>
#include "Scene5g.h"
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"

// Imgui stuff
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"


Scene5g::Scene5g() :plane{ nullptr }, shader{ nullptr }, mesh{ nullptr },
drawInWireMode{ true } {
	Debug::Info("Created Scene5g: ", __FILE__, __LINE__);
}

Scene5g::~Scene5g() {
	Debug::Info("Deleted Scene5g: ", __FILE__, __LINE__);
}

bool Scene5g::OnCreate() {
	Debug::Info("Loading assets Scene5g: ", __FILE__, __LINE__);
	plane = new Body();
	plane->OnCreate();

	mesh = new Mesh("meshes/Plane.obj");
	mesh->OnCreate();

	shader = new Shader("shaders/tessVert.glsl"
		, "shaders/tessFrag.glsl"
		, "shaders/tessCtrl.glsl"
		, "shaders/tessEval.glsl"
	);
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}
	
	terrainHeight = new Texture();
	if (terrainHeight->LoadMultipleImages("textures/terrainHeight.png", 0) == false) {
		std::cerr << "OH NO!\n";
		return false;
	}
	terrainNormal = new Texture();
	if (terrainNormal->LoadMultipleImages("textures/terrainNormal.png", 1) == false) {
		std::cerr << "OH NO!\n";
		return false;
	}
	terrainDiffuse = new Texture();
	if (terrainDiffuse->LoadMultipleImages("textures/terrainDiffuse.png", 2) == false) {
		std::cerr << "OH NO!\n";
		return false;
	}
	

	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 14.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	modelMatrix.loadIdentity();

	tessLevel = 1;



	return true;
}

void Scene5g::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);
	plane->OnDestroy();
	delete plane;

	mesh->OnDestroy();
	delete mesh;

	shader->OnDestroy();
	delete shader;

	delete terrainHeight;
	delete terrainNormal;
	delete terrainDiffuse;


}

void Scene5g::HandleEvents(const SDL_Event& sdlEvent) {
	switch (sdlEvent.type) {
	case SDL_EVENT_KEY_DOWN:
		switch (sdlEvent.key.scancode) {
		case SDL_SCANCODE_W:
			drawInWireMode = !drawInWireMode;
			break;
		case SDL_SCANCODE_UP:
			tessLevel++;
			break;
		case SDL_SCANCODE_DOWN:
			if (tessLevel > 1) {
				tessLevel--;
				break;
			}
			
		default:
			break;
		}
		break;
	}
}

void Scene5g::Update(const float deltaTime) {
	// Start the Dear ImGui frame
	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplSDL3_NewFrame();
	//ImGui::NewFrame();

	//// This is the overwhelming demo
	////ImGui::ShowDemoWindow();
	//ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
	//ImGui::SliderFloat("tessLevel", &tessLevel, 1.0f, 32.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//ImGui::End();
}

void Scene5g::Render() const {
	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	camera.Render();

	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, modelMatrix);
	glUniform1f(shader->GetUniformID("tesslevel"), tessLevel);
	glUniform1f(shader->GetUniformID("density"), densityLevel);



	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrainHeight->getTextureID());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrainNormal->getTextureID());

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, terrainDiffuse->getTextureID());


	
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, 1);
	glBindTexture(GL_TEXTURE_2D, 2);


	//mesh->Render(GL_PATCHES); // Thanks to Ethan for reminding me this should be patches
	glUseProgram(0);

	/*ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/
}