#include <glew.h>
#include <iostream>
#include <SDL.h>
#include <SDL3/SDL_events.h>
#include "Scene4g.h"
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
// Imgui stuff
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"


Scene4g::Scene4g() :plane{ nullptr }, shader{ nullptr }, mesh{ nullptr },
drawInWireMode{ true } {
	Debug::Info("Created Scene4g: ", __FILE__, __LINE__);
}

Scene4g::~Scene4g() {
	Debug::Info("Deleted Scene4g: ", __FILE__, __LINE__);
}

bool Scene4g::OnCreate() {
	Debug::Info("Loading assets Scene4g: ", __FILE__, __LINE__);
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

	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 14.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	modelMatrix.loadIdentity();





	return true;
}

void Scene4g::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);
	plane->OnDestroy();
	delete plane;

	mesh->OnDestroy();
	delete mesh;

	shader->OnDestroy();
	delete shader;


}

void Scene4g::HandleEvents(const SDL_Event& sdlEvent) {
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

void Scene4g::Update(const float deltaTime) {
	//// Start the Dear ImGui frame
	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplSDL3_NewFrame();
	//ImGui::NewFrame();

	//// This is the overwhelming demo
	//ImGui::ShowDemoWindow();
}

void Scene4g::Render() const {
	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, modelMatrix);
	mesh->Render(GL_PATCHES); // Thanks to Ethan for reminding me this should be patches
	glUseProgram(0);

	//ImGui::Render();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}