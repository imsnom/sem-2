// Scene0pr for game production game jam assignment
#include <glew.h>
#include <iostream>
#include <SDL.h>
#include <SDL3/SDL_events.h>
#include "Scene0pr.h"
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include "Collision.h"
#include <QMath.h>

Scene0pr::Scene0pr() :
cueBall{ nullptr },
shader{ nullptr },
mesh{ nullptr },
drawInWireMode{ false },
targetBall{ nullptr } {
	Debug::Info("Created Scene0pr: ", __FILE__, __LINE__);
}

Scene0pr::~Scene0pr() {
	Debug::Info("Deleted Scene0pr: ", __FILE__, __LINE__);
}
 
bool Scene0pr::OnCreate() {
	srand(time(0));
	Debug::Info("Loading assets Scene0pr: ", __FILE__, __LINE__);
	cueBall = new Body();
	cueBall->OnCreate();
	cueBall->pos = Vec3(0, 1, 9);
	cueBall->radius = 1.0f;
	cueBall->vel = Vec3(0, 0, 0);

	targetBall = new Body();
	targetBall->OnCreate();
	targetBall->pos = Vec3(-0.7, 1, 0);
	targetBall->radius = 1.0f;

	float randX = rand() % 6;
	float randY = rand() % 6;
	float randZ = rand() % 6 * -1;

	goalBall = new Body();
	goalBall->OnCreate();
	//std::cout << randX << ", " << randY << ", " << randZ << std::endl;
	goalBall->pos = Vec3(randX, randY, randZ);
	goalBall->radius = 1.0f;

	plane = new Body();
	plane->OnCreate();


	mesh = new Mesh("meshes/Plane.obj");
	mesh->OnCreate();

	sphereMesh = new Mesh("meshes/Sphere.obj");
	sphereMesh->OnCreate();

	earthTexture = new Texture();
	if (earthTexture->LoadImage("textures/earthclouds_texture.jpg") == false) {
		std::cerr << "OH NO!\n";
		return false;
	}
	eyeTexture = new Texture();
	if (eyeTexture->LoadImage("textures/evilEye.jpg") == false) {
		std::cerr << "OH NO!\n";
		return false;
	}
	red = new Texture();
	if (red->LoadImage("textures/red.jpg") == false) {
		std::cerr << "OH NO!\n";
		return false;
	}

	shader = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	eyeModelMatrix.loadIdentity();

	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	//viewMatrix = MMath::lookAt(Vec3(0.0f, 1.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	// instead of using the view matrix, let's use the camera's position and orientation
	cameraPos = Vec3(0.0f, 1.0f, 16.0f);
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

void Scene0pr::OnDestroy() {
	Debug::Info("Deleting assets Scene0pr: ", __FILE__, __LINE__);
	cueBall->OnDestroy();
	delete cueBall;

	targetBall->OnDestroy();
	delete targetBall;

	goalBall->OnDestroy();
	delete goalBall;

	mesh->OnDestroy();
	delete mesh;

	sphereMesh->OnDestroy();
	delete sphereMesh;

	shader->OnDestroy();
	delete shader;


}

void Scene0pr::HandleEvents(const SDL_Event& sdlEvent) {
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

void Scene0pr::Update(const float deltaTime) {

	// switch off rolling when starting assignment two
	if (COLLISION::Detection(*cueBall, *targetBall)) {
		std::cout << "COLLIDED!\n";

		COLLISION::Response(*cueBall, *targetBall);


	}
	else if (COLLISION::Detection(*targetBall, *goalBall)) {
		std::cout << "hit! " << std::endl;

		return;
	}

	//cueBall->vel.print();
	cueBall->UpdatePos(deltaTime);
	targetBall->UpdatePos(deltaTime);
	//cameraPos = cueBall->pos + Vec3(0, 0, 7);


	Quaternion qInitial = cameraOrientation;
	Quaternion qFinal = trackball.getQuat();
	Quaternion changeInRotation = qFinal * QMath::conjugate(qInitial);
	// fancy work to inverse a quaternion is "conjugate"


	cameraOrientation = trackball.getQuat();


	cameraPos -= cueBall->pos;

	cameraPos = QMath::rotate(cameraPos, changeInRotation);

	cameraPos += cueBall->pos;

	eyeModelMatrix = MMath::translate(0.6f, 0.25f, 0.9f) *
		MMath::scale(0.3f, 0.3f, 0.3f);


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

void Scene0pr::Render() const {
	glEnable(GL_DEPTH_TEST);
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
	//glUniform4fv(shader->GetUniformID("color"), 1, green);
	//mesh->Render(GL_TRIANGLES);
	Matrix4 rotation = MMath::inverse(MMath::toMatrix4(trackball.getQuat())) *
		MMath::rotate(-90.0f, Vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, targetBall->GetModelMatrix() * rotation);
	//Vec4 red = Vec4(1, 0, 0, 0);
	//glUniform4fv(shader->GetUniformID("color"), 1, red);
	glBindTexture(GL_TEXTURE_2D, eyeTexture->getTextureID());
	sphereMesh->Render(GL_TRIANGLES);


	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, cueBall->GetModelMatrix());

	//Vec4 white = Vec4(1, 1, 1, 0);
	//glUniform4fv(shader->GetUniformID("color"), 1, white);
	glBindTexture(GL_TEXTURE_2D, red->getTextureID());
	sphereMesh->Render(GL_TRIANGLES);

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, goalBall->GetModelMatrix());
	glBindTexture(GL_TEXTURE_2D, earthTexture->getTextureID());

	//Vec4 purple = Vec4(1, 0, 1, 0);
	//glUniform4fv(shader->GetUniformID("color"), 1, purple);
	sphereMesh->Render(GL_TRIANGLES);


	glUseProgram(0);
}




