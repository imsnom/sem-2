#include <glew.h>
#include <iostream>
#include <SDL.h>
#include <SDL3/SDL_events.h>
#include "Scene3p.h"
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include <VMath.h>
#include "Collision.h"
#include <QMath.h>

Scene3p::Scene3p() :
	jellyfishHead{ nullptr }
	, shader{ nullptr }
	, numSpheresPerAnchor{ 10 }
	, spacing{ 1.0f }
{
	Debug::Info("Created Scene3p: ", __FILE__, __LINE__);
}

Scene3p::~Scene3p() {
	Debug::Info("Deleted Scene3p: ", __FILE__, __LINE__);
}

bool Scene3p::OnCreate() {
	Debug::Info("Loading assets Scene3p: ", __FILE__, __LINE__);
	jellyfishHead = new Body();
	jellyfishHead->OnCreate();
	jellyfishHead->pos = Vec3(-1.5, 4, -25);
	jellyfishHead->radius = 6.0f;
	jellyfishHead->vel = Vec3(0, 0, 0);
	// Don't forget, the mesh is in the body object
	jellyfishHead->SetMesh("meshes/Sphere.obj");

	const int numAnchors = 10;
	Vec3 anchorPos(-6.0f, 0.0f, -25);
	for (int i = 0; i < numAnchors; i++) {
		anchors.push_back(new Body());
		anchors[i]->pos = anchorPos;
		anchors[i]->radius = 0.5f;
		// Move the anchor position for the next swing through this loop 
		anchorPos += Vec3(spacing, 0, 0);
		// We will use the jellyfish sphere mesh for all our anchors
		// So I won't bother with setting up meshes for each anchor

		
	}
	for (int i = 0; i < numSpheresPerAnchor; i++) {
		// 
		Vec3 pos = anchors[0]->pos + Vec3(0, -spacing * (i+ 1), 0); // possible bug here
		tentacleSpheres.push_back(new Body());
		tentacleSpheres[i]->pos = pos;
		tentacleSpheres[i]->radius = 0.2f;
		tentacleSpheres[i]->mass = 1.0f;
	}


	shader = new Shader("shaders/defaultVert.glsl", "shaders/colorFrag.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	// Instead of using the view matrix, lets use the camera's position and orientation
	cameraPos = Vec3(0.0f, 0.0f, 5.0f);
	cameraOrientation = QMath::angleAxisRotation(0, Vec3(1, 0, 0));
	Matrix4 T = MMath::translate(cameraPos);         // T for Translation Matrix
	Matrix4 R = MMath::toMatrix4(cameraOrientation); // R for Rotation Matrix
	// Umer pointed at some cat pictures to show
	// that the view matrix is first an inverse translation
	//                         then and inverse rotation
	viewMatrix = MMath::inverse(R) * MMath::inverse(T);

	return true;
}

void Scene3p::OnDestroy() {
	Debug::Info("Deleting assets Scene3p: ", __FILE__, __LINE__);
	jellyfishHead->OnDestroy();
	delete jellyfishHead;

	
	for (Body* tentacleSphere : tentacleSpheres) {
		tentacleSphere->OnDestroy();
		delete tentacleSphere;
	}
	for (Body* anchor : anchors) {
		anchor->OnDestroy();
		delete anchor;
	}
	shader->OnDestroy();
	delete shader;

}

void Scene3p::HandleEvents(const SDL_Event& sdlEvent) {
	trackball.HandleEvents(sdlEvent);
	switch (sdlEvent.type) {
	case SDL_EVENT_KEY_DOWN:
		switch (sdlEvent.key.scancode) {
		case SDL_SCANCODE_UP:
			break;
		case SDL_SCANCODE_DOWN:

			break;
		case SDL_SCANCODE_LEFT:

			break;
		case SDL_SCANCODE_RIGHT:

			break;
		case SDL_SCANCODE_SPACE:
			break;

		case SDL_SCANCODE_W:
			break;
		default:
			break;
		}
		break;
	}
}

void Scene3p::Update(const float deltaTime) {

	float g = 1;
	for (int i = 0; i < tentacleSpheres.size(); i++) {
		Vec3 gravityForce = Vec3(0, -g, 0.0f) * tentacleSpheres[i]->mass;
		float dragCoeff = 1.5f;
		Vec3 dragForce = -dragCoeff * tentacleSpheres[i]->vel;
		tentacleSpheres[i]->ApplyForce(gravityForce + dragForce);
		tentacleSpheres[i]->UpdateVel(deltaTime);
		float slope = -1;
		float yIntercept = 1;
		tentacleSpheres[i]->StraightLineConstraint(slope, yIntercept, deltaTime);
		tentacleSpheres[i]->UpdatePos(deltaTime);

	}


	// Let's set up an orbit camera using the trackball
	// Rotate the position
	// Based on Umer's scribbles, we just need to divide the quaternions
	Quaternion qInitial = cameraOrientation;
	Quaternion qFinal = trackball.getQuat();
	// Fancy word to inverse a quaternion is "conjugate"
	Quaternion changeInRotation = qFinal * QMath::conjugate(qInitial); // qFinal / qInitial;
	cameraOrientation = trackball.getQuat();

	// We want to rotate around the cueBall
	// Following Umer's scribbles we need three steps
	// STEP 1: Translate so that the ball is at the origin
	cameraPos -= jellyfishHead->pos;
	// STEP 2: Rotate now
	cameraPos = QMath::rotate(cameraPos, changeInRotation);
	// STEP 3: Translate back
	cameraPos += jellyfishHead->pos;

	Matrix4 T = MMath::translate(cameraPos);         // T for Translation Matrix
	Matrix4 R = MMath::toMatrix4(cameraOrientation); // R for Rotation Matrix

	viewMatrix = MMath::inverse(R) * MMath::inverse(T);
}

void Scene3p::Render() const {
	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, jellyfishHead->GetModelMatrix());
	// Umer likes the jellyfish from Spongebob
	Vec3 color = Vec3(227, 152, 217) / 255; // a nice pink
	glUniform4fv(shader->GetUniformID("color"), 1, Vec4(color, 0));
	jellyfishHead->GetMesh()->Render(GL_TRIANGLES);



	for (Body* anchor : anchors) {
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, anchor->GetModelMatrix());
		jellyfishHead->GetMesh()->Render(GL_TRIANGLES);
	}
	for (Body* tentacleSphere : tentacleSpheres) {
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, tentacleSphere->GetModelMatrix());
		jellyfishHead->GetMesh()->Render(GL_TRIANGLES);
	}

	glUseProgram(0);
}
