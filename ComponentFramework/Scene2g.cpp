#include <glew.h>
#include <iostream>
#include <SDL.h>
#include <SDL3/SDL_events.h>
#include "Scene2g.h"
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include "Texture.h"

Scene2g::Scene2g() ://sphere{nullptr}, 
					shader{nullptr}, 
					mesh{nullptr},
					drawInWireMode{false} {
	Debug::Info("Created Scene2: ", __FILE__, __LINE__);
}

Scene2g::~Scene2g() {
	Debug::Info("Deleted Scene2: ", __FILE__, __LINE__);
}

bool Scene2g::OnCreate() {
	Debug::Info("Loading assets Scene2: ", __FILE__, __LINE__);
	
	eyeBall = new Mesh("meshes/Sphere.obj");
	eyeBall->OnCreate();

	eyeBall_2 = new Mesh("meshes/Sphere.obj");
	eyeBall_2->OnCreate();

	skullMesh = new Mesh("meshes/Skull.obj");
	skullMesh->OnCreate();

	
	shader = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	projection = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	view = MMath::lookAt(Vec3(0.0f, 0.0f, 7.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	view.print("view matrix ");


	/*earthModelMatrix = MMath::translate(0.0f, 0.0f, -80.0f);
	earthModelMatrix.loadIdentity();*/

	skullModelMatrix.loadIdentity();

	eyeBallModelMatrix.loadIdentity();

	eyeBallModelMatrix_2.loadIdentity();



	lightPos = Vec3(10.0f, 0.0f, 0.0f); // sets the position of the light
	//lightPos_2 = Vec3(-10.0f, 0.0f, 0.0f);
	
	//colour = Vec4(0.7f, 0.6f, 0.2f, 0.0f);


	/*earthTexture = new Texture();
	if (earthTexture->LoadImage("textures/earthclouds_texture.jpg") == false) {
		std::cerr << "OH NO!\n";
		return false;
	}
	moonTexture = new Texture();
	if (moonTexture->LoadImage("textures/moon_texture.jpg") == false) {
		std::cerr << "OH NO!\n";
		return false;
	}*/
	skullTexture = new Texture();
	if (skullTexture->LoadImage("textures/skull_texture.jpg") == false) {
		std::cerr << "OH NO!\n";
		return false;
	}
	eyeBallTexture = new Texture();
	if (eyeBallTexture->LoadImage("textures/evilEye.jpg") == false) {
		std::cerr << "OH NO!\n";
		return false;
	}
	


	return true;
}

void Scene2g::OnDestroy() {
	Debug::Info("Deleting assets Scene2: ", __FILE__, __LINE__);
	/*sphere->OnDestroy();
	delete sphere;*/

	mesh->OnDestroy();
	delete mesh;

	skullMesh->OnDestroy();
	delete skullMesh;

	shader->OnDestroy();
	delete shader;

	eyeBall->OnDestroy();
	delete eyeBall;

	eyeBall_2->OnDestroy();
	delete eyeBall_2;
	
	/*delete earthTexture;

	delete moonTexture;*/

	delete skullTexture;

	delete eyeBallTexture;
	
	
}

void Scene2g::HandleEvents(const SDL_Event &sdlEvent) {
	trackball.HandleEvents(sdlEvent);
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

void Scene2g::Update(const float deltaTime) {
	static float totalTime = 0.0f;
	totalTime += deltaTime;

	skullModelMatrix = MMath::toMatrix4(trackball.getQuat());

	//rotation = MMath::lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(3.0f, 0.0f, -7.0f), Vec3(0.0f, 1.0f, 0.0f));


	/*earthModelMatrix =
		MMath::rotate(10 * totalTime, Vec3(0.0f, 1.0f, 0.0f)) *
		MMath::rotate(-90.0f, Vec3(1.0f, 0.0f, 0.0f));

	moonModelMatrix =
		MMath::translate(3.0f, 0.0f, 0.0f) *
		MMath::scale(0.24f, 0.24f, 0.24f) *
		MMath::rotate(20.0f * totalTime, Vec3(0.0f, 1.0f, 0.0f));*/


	/*skullModelMatrix = MMath::translate(3.0f, 0.0f, 0.0f) *
		MMath::scale(0.24f, 0.24f, 0.24f);*/

	eyeBallModelMatrix = MMath::translate(0.6f, 0.25f, 0.9f) *
						 MMath::scale(0.3f, 0.3f, 0.3f);

	eyeBallModelMatrix_2 = MMath::translate(-0.6f, 0.25f, 0.9f) *
						   MMath::scale(0.3f, 0.3f, 0.3f);

}

void Scene2g::Render() const {
	glEnable(GL_DEPTH_TEST);
	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(drawInWireMode){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glUseProgram(shader->GetProgram());

	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projection);
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, view);
	glUniform3fv(shader->GetUniformID("lightPos"), 1, lightPos); // sends light position to the gpu
	

	
	Matrix4 rotation = MMath::inverse(MMath::toMatrix4(trackball.getQuat())) * 
					   MMath::rotate(-90.0f, Vec3(0.0f, 1.0f, 0.0f)); // the eyes have to be oriented towards the origin here rather than in update



	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, skullModelMatrix);
	glBindTexture(GL_TEXTURE_2D, skullTexture->getTextureID());
	skullMesh->Render(GL_TRIANGLES);

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, skullModelMatrix * eyeBallModelMatrix * rotation);
	glBindTexture(GL_TEXTURE_2D, eyeBallTexture->getTextureID());
	eyeBall->Render(GL_TRIANGLES);

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, skullModelMatrix * eyeBallModelMatrix_2 * rotation);
	glBindTexture(GL_TEXTURE_2D, eyeBallTexture->getTextureID());
	eyeBall_2->Render(GL_TRIANGLES);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glUseProgram(0);
}



	
