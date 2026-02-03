// Scene1p for first physics assignment
#include <glew.h>
#include <iostream>
#include <SDL.h>
#include <SDL3/SDL_events.h>
#include "Scene1p.h"
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"

Scene1p::Scene1p() :sphere{ nullptr }, shader{ nullptr }, mesh{ nullptr },
drawInWireMode{ true } {
	Debug::Info("Created Scene1p: ", __FILE__, __LINE__);
}

Scene1p::~Scene1p() {
	Debug::Info("Deleted Scene1p: ", __FILE__, __LINE__);
}

bool Scene1p::OnCreate() {
	Debug::Info("Loading assets Scene1p: ", __FILE__, __LINE__);
	sphere = new Body();
	sphere->OnCreate();
	sphere->angularVel = Vec3(0, 0, 0);
	//sphere->vel = Vec3(0, 0, 1);
	sphere->radius = 1.0f;

	plane = new Body();
	plane->OnCreate();
	

	mesh = new Mesh("meshes/Plane.obj");
	mesh->OnCreate();

	sphereMesh = new Mesh("meshes/Sphere.obj");
	sphereMesh->OnCreate();

	shader = new Shader("shaders/defaultVert.glsl", "shaders/defaultFrag.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	// Rotate!
	float angleDegrees = 90;
	Vec3 axis = Vec3(1, 0, 0);
	modelMatrix = MMath::rotate(angleDegrees, axis);
	sphereModelMatrix = MMath::translate(Vec3(0, 1, 0));
	planeNormal = Vec3(0, 1, 0);
	return true;
}

void Scene1p::OnDestroy() {
	Debug::Info("Deleting assets Scene1p: ", __FILE__, __LINE__);
	sphere->OnDestroy();
	delete sphere;

	mesh->OnDestroy();
	delete mesh;

	sphereMesh->OnDestroy();
	delete sphereMesh;

	shader->OnDestroy();
	delete shader;


}

void Scene1p::HandleEvents(const SDL_Event& sdlEvent) {
	const float angleDeg = 5;
	switch (sdlEvent.type) {
	case SDL_EVENT_KEY_DOWN:
		switch (sdlEvent.key.scancode) {
			// Cooper says we should use the arrow keys to rotate the plane
		case SDL_SCANCODE_UP:
			// Lets make the ball roll away from us (up to you)
		{
			Vec3 axis = Vec3(-1, 0, 0);
			Matrix4 rotation = MMath::rotate(angleDeg, axis);
			// Update the model matrix
			modelMatrix *= rotation;
			// update the normal
			planeNormal = rotation * planeNormal;
		}

		break;
		case SDL_SCANCODE_DOWN:

			break;
		case SDL_SCANCODE_LEFT:

			break;
		case SDL_SCANCODE_RIGHT:

			break;




		case SDL_SCANCODE_W:
			break;
		default:
			break;
		}
		break;
	}
}

void Scene1p::Update(const float deltaTime) {

	Vec3 linearVel = sphere->angularVel;

	
	

	// part one of assignment one
	// can you make the ball move based on angular velocity using umer's scribbles on the board
	// linear velocity = angular velocity CROSS (radius * plane normal)
	// sphere->update(deltaTime)

	float angleRadOfPlane; // acos(up vector DOT plane normal)
	float distanceToPivot = radius * sin(angleRadOfPlane); // radius * sin(angleRad)
	float weight = sphere->mass * SDL_STANDARD_GRAVITY; // mass * g
	Vec3 directionOfTorque; // up vector CROSS plane normal
	Vec3 torque = distanceToPivot * weight * directionOfTorque; // distanceToPivot * weight * direction
	

	sphere->ApplyTorque(torque);
	// now that angular acceleration has been calculated above we need to update angular velocity
	sphere->UpdateAngularVel(deltaTime);

	sphere->UpdateOrientation(deltaTime);
	sphere->UpdatePos(deltaTime);

	//sphere->vel = VMath::cross(sphere->angularVel, (sphere->radius * Vec3(0.0f, 1.0f, 0.0f)));

	//Matrix4 T = MMath::translate(Vec3(0, 1, 0));
	Matrix4 T = MMath::translate(Vec3(0, sphere->radius, 0) + sphere->pos);

	Matrix4 R = MMath::toMatrix4(sphere->orientation);

	Matrix4 S = MMath::scale(sphere->radius, sphere->radius, sphere->radius);

	sphereModelMatrix = T * R * S;
	

}

void Scene1p::Render() const {
	//glEnable(GL_DEPTH_TEST);
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
	mesh->Render(GL_TRIANGLES);
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, sphereModelMatrix);
	sphereMesh->Render(GL_TRIANGLES);
	glUseProgram(0);
}




