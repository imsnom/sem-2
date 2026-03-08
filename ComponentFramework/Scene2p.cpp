// Scene2p for second physics assignment
#include <glew.h>
#include <iostream>
#include <SDL.h>
#include <SDL3/SDL_events.h>
#include "Scene2p.h"
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include "Collision.h"
#include <QMath.h>

Scene2p::Scene2p() :cueBall{ nullptr },
shader{ nullptr }, 
mesh{ nullptr },
drawInWireMode{ true }, 
targetBall{ nullptr } {
	Debug::Info("Created Scene2p: ", __FILE__, __LINE__);
}

Scene2p::~Scene2p() {
	Debug::Info("Deleted Scene2p: ", __FILE__, __LINE__);
}

bool Scene2p::OnCreate() {
	Debug::Info("Loading assets Scene2p: ", __FILE__, __LINE__);
	cueBall = new Body();
	cueBall->OnCreate();
	cueBall->pos = Vec3(0, 1, 3);
	cueBall->radius = 1.0f;
	cueBall->vel = Vec3(0, 0, 0);

	targetBall = new Body();
	targetBall->OnCreate();
	targetBall->pos = Vec3(-0.7, 1, -6);
	targetBall->radius = 1.0f;

	plane = new Body();
	plane->OnCreate();
	

	mesh = new Mesh("meshes/Plane.obj");
	mesh->OnCreate();

	sphereMesh = new Mesh("meshes/Sphere.obj");
	sphereMesh->OnCreate();

	shader = new Shader("shaders/defaultVert.glsl", "shaders/colorFrag.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	//viewMatrix = MMath::lookAt(Vec3(0.0f, 1.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	// instead of using the view matrix, let's use the camera's position and orientation
	cameraPos = Vec3(0.0f, 1.0f, 10.0f);
	cameraOrientation = QMath::angleAxisRotation(0, Vec3(0, 1, 0));
	Matrix4 T = MMath::translate(cameraPos);		 // T for translation matrix
	Matrix4 R = MMath::toMatrix4(cameraOrientation); // R for rotation matrix

	viewMatrix = MMath::inverse(R) * MMath::inverse(T);


	// Rotate!
	float angleDegrees = 90;
	axis = Vec3(1, 0, 0);
	modelMatrix = MMath::rotate(angleDegrees, axis) * MMath::scale(2, 2, 2);
	//cueBall->GetModelMatrix() = MMath::translate(Vec3(0, 1, 0));
	planeNormal = Vec3(0, 1, 0); 
	return true;
}

void Scene2p::OnDestroy() {
	Debug::Info("Deleting assets Scene2p: ", __FILE__, __LINE__);
	cueBall->OnDestroy();
	delete cueBall;

	targetBall->OnDestroy();
	delete targetBall;

	mesh->OnDestroy();
	delete mesh;

	sphereMesh->OnDestroy();
	delete sphereMesh;

	shader->OnDestroy();
	delete shader;


}

void Scene2p::HandleEvents(const SDL_Event& sdlEvent) {
	const float angleDeg = 5;
	trackball.HandleEvents(sdlEvent);

	switch (sdlEvent.type) {
	case SDL_EVENT_KEY_DOWN:
		switch (sdlEvent.key.scancode) {
			// Cooper says we should use the arrow keys to rotate the plane
		case SDL_SCANCODE_UP:
			// Lets make the ball roll away from us (up to you)
		{
			axis = Vec3(-1, 0, 0);
			rotation = MMath::rotate(angleDeg, axis);
			// Update the model matrix
			modelMatrix *= rotation;
			// update the normal
			planeNormal = rotation * planeNormal;
		}

		break;
		case SDL_SCANCODE_DOWN:
			axis = Vec3(1, 0, 0);
			rotation = MMath::rotate(angleDeg, axis);
			modelMatrix *= rotation;
			planeNormal = rotation * planeNormal;
			break;
		case SDL_SCANCODE_LEFT:
			axis = Vec3(0, 1, 0);
			rotation = MMath::rotate(angleDeg, axis);
			modelMatrix *= rotation;
			planeNormal = rotation * planeNormal;
			break;
		case SDL_SCANCODE_RIGHT:
			axis = Vec3(0, -1, 0);
			rotation = MMath::rotate(angleDeg, axis);
			modelMatrix *= rotation;
			planeNormal = rotation * planeNormal;
			break;

		case SDL_SCANCODE_SPACE:
		{
			Vec4 whackInCameraSpace(0, 0, -1, 0); // all directions have a w=0
			Vec3 whackInWorldSpace = MMath::inverse(viewMatrix) * whackInCameraSpace;
			cueBall->vel += whackInWorldSpace;
		}
		break;


		case SDL_SCANCODE_W:
			break;
		default:
			break;
		}
		break;
	}
}

void Scene2p::Update(const float deltaTime) {

	// switch off rolling when starting assignment two
	if (COLLISION::Detection(*cueBall, *targetBall)) {
		std::cout << "COLLIDED!\n";
		
		COLLISION::Response(*cueBall, *targetBall);

	}

	cueBall->vel.print();
	cueBall->UpdatePos(deltaTime);
	targetBall->UpdatePos(deltaTime);

	Quaternion qInitial = cameraOrientation;
	Quaternion qFinal = trackball.getQuat();
	Quaternion changeInRotation = qFinal * QMath::conjugate(qInitial);
	// fancy work to inverse a quaternion is "conjugate"

	cameraOrientation = trackball.getQuat();
	

	cameraPos -= cueBall->pos;

	cameraPos = QMath::rotate(cameraPos, changeInRotation);

	cameraPos += cueBall->pos;


	Matrix4 T = MMath::translate(cameraPos);         // T for Translation Matrix
	Matrix4 R = MMath::toMatrix4(cameraOrientation); // R for Rotation Matrix
	viewMatrix = MMath::inverse(R) * MMath::inverse(T);
	

	// part one of assignment one
	// can you make the ball move based on angular velocity using umer's scribbles on the board
	// linear velocity = angular velocity CROSS (radius * plane normal)
	// sphere->update(deltaTime)

	//float angleRadOfPlane = acos(VMath::dot(axis, planeNormal)); // acos(up vector DOT plane normal)
	//float distanceToPivot = cueBall->radius * sin(angleRadOfPlane); // radius * sin(angleRad)
	//float weight = cueBall->mass * SDL_STANDARD_GRAVITY; // mass * g
	//Vec3 directionOfTorque = VMath::cross(axis, planeNormal); // up vector CROSS plane normal
	//Vec3 torque = distanceToPivot * weight * directionOfTorque; // distanceToPivot * weight * direction
	//

	//cueBall->ApplyTorque(torque);
	//// now that angular acceleration has been calculated above we need to update angular velocity
	//cueBall->UpdateAngularVel(deltaTime);

	//cueBall->UpdateOrientation(deltaTime);
	//cueBall->UpdatePos(deltaTime);

	////sphere->vel = VMath::cross(sphere->angularVel, (sphere->radius * Vec3(0.0f, 1.0f, 0.0f)));

	////Matrix4 T = MMath::translate(Vec3(0, 1, 0));
	//Matrix4 T = MMath::translate(Vec3(0, cueBall->radius, 0) + cueBall->pos);

	//Matrix4 R = MMath::toMatrix4(cueBall->orientation);

	//Matrix4 S = MMath::scale(cueBall->radius, cueBall->radius, cueBall->radius);

	//cueBall->GetModelMatrix() = T * R * S;
	

}

void Scene2p::Render() const {
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
	Vec4 green = Vec4(0, 1, 0, 0);
	glUniform4fv(shader->GetUniformID("color"), 1, green);
	mesh->Render(GL_TRIANGLES);
	

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, targetBall->GetModelMatrix());
	Vec4 red = Vec4(1, 0, 0, 0);
	glUniform4fv(shader->GetUniformID("color"), 1, red);
	sphereMesh->Render(GL_TRIANGLES);

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, cueBall->GetModelMatrix());
	Vec4 white = Vec4(1, 1, 1, 0);
	glUniform4fv(shader->GetUniformID("color"), 1, white);
	sphereMesh->Render(GL_TRIANGLES);
	glUseProgram(0);
}




