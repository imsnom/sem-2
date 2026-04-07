#include <glew.h>
#include <iostream>
#include <SDL.h>
#include <SDL3/SDL_events.h>
#include "Scene4p.h"
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include <VMath.h>
#include "Collision.h"
#include <QMath.h>
// fancy geometric algebra stuff
#include <DQMath.h>
#include <Join.h>
#include <Meet.h>

using namespace MATHEX;

Scene4p::Scene4p() :
	shader{ nullptr }
	, target{ nullptr }
{
	Debug::Info("Created Scene4p: ", __FILE__, __LINE__);
}

Scene4p::~Scene4p() {
	Debug::Info("Deleted Scene4p: ", __FILE__, __LINE__);
}

bool Scene4p::OnCreate() {
	Debug::Info("Loading assets Scene4p: ", __FILE__, __LINE__);

	// Umer will set up the head, neck, pelvis, and left arm. You can do the rest
	for (int i = 0; i < 17; i++) {
		joints.push_back(new Body());
		joints[i]->OnCreate();
		joints[i]->radius = 0.1;
		if (i == 0) {
			// Just set the mesh for the head (sorry I am lazy)
			joints[0]->SetMesh("meshes/Sphere.obj");
		}
	}

	// Make the head bigger
	joints[0]->radius = 0.4f;

	joints[0]->pos = Vec3(0, 4, 0); // head
	joints[1]->pos = Vec3(0, 3, 0); // neck
	joints[2]->pos = Vec3(0, 1, 0); // pelvis

	// Left arm
	joints[3]->pos = Vec3(1, 3, 0); // shoulder
	joints[4]->pos = Vec3(1.5, 2, 0); // elbow
	joints[5]->pos = Vec3(1, 1, 0); // hand

	// Left leg
	joints[6]->pos = Vec3(0.5f, 0.8f, 0); // hip
	joints[7]->pos = Vec3(0.5f, -1.2f, 0); // knee
	joints[8]->pos = Vec3(0.5f, -3.2f, 0); // ankle
	joints[9]->pos = Vec3(0.8f, -3.2f, 0); // toe

	// Right arm
	joints[10]->pos = joints[3]->pos + Vec3(-2, 0, 0); // shoulder
	joints[11]->pos = joints[4]->pos + Vec3(-3, 0, 0); // elbow
	joints[12]->pos = joints[5]->pos + Vec3(-2, 0, 0); // hand

	// Right leg
	joints[13]->pos = joints[6]->pos + Vec3(-1, 0, 0); // hip
	joints[14]->pos = joints[7]->pos + Vec3(-1, 0, 0); // knee
	joints[15]->pos = joints[8]->pos + Vec3(-1, 0, 0); // ankle
	joints[16]->pos = joints[9]->pos + Vec3(-1.6f, 0, 0); // toe


	target = new Body();
	target->pos = Vec3(4.0f, 4.0f, 0.0f);
	target->radius = 0.1f;


	shader = new Shader("shaders/defaultVert.glsl", "shaders/colorFrag.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	// Instead of using the view matrix, lets use the camera's position and orientation
	cameraPos = Vec3(0.0f, 0.0f, 12.0f);
	cameraOrientation = QMath::angleAxisRotation(0, Vec3(1, 0, 0));
	Matrix4 T = MMath::translate(cameraPos);         // T for Translation Matrix
	Matrix4 R = MMath::toMatrix4(cameraOrientation); // R for Rotation Matrix
	// Umer pointed at some cat pictures to show
	// that the view matrix is first an inverse translation
	//                         then and inverse rotation
	viewMatrix = MMath::inverse(R) * MMath::inverse(T);

	return true;
}

void Scene4p::OnDestroy() {
	Debug::Info("Deleting assets Scene4p: ", __FILE__, __LINE__);
	// Use a cool for loop for vector lists
	for (Body* joint : joints) {
		joint->OnDestroy();
		delete joint;
		joint = nullptr;
	}

	target->OnDestroy();
	target = nullptr;

	shader->OnDestroy();
	delete shader;
}

void Scene4p::HandleEvents(const SDL_Event& sdlEvent) {
	//trackball.HandleEvents(sdlEvent);
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
		case SDL_SCANCODE_S:
			break;
		

		default:
			break;

		}
		break;
	case SDL_EVENT_MOUSE_MOTION:
		// for positions, set w to 1 
		Vec4 mousePosPixel = Vec4(sdlEvent.button.x, sdlEvent.button.y, 0, 1);
		Matrix4 pixelToNDC = MMath::inverse(MMath::NDCtoViewport(1280, 720)); 
		Vec4 mousePosNDC = pixelToNDC * mousePosPixel;
		// Front of the NDC box (near plane) has a z = -1
		mousePosNDC.z = -1;

		Matrix4 NDCToCamera = MMath::inverse(projectionMatrix);
		Vec4 mousePosCamera = NDCToCamera * mousePosNDC;
		mousePosCamera = VMath::perspectiveDivide(mousePosCamera);
		
		Matrix4 cameraToWorld = MMath::inverse(viewMatrix);
		Vec4 mousePosWorld = cameraToWorld * mousePosCamera;

		// Let's ray trace with fancy geometric algebra

		// First, make a line frmo the camera pos to the mouse pos
		
		DualQuat line = join(Vec4(cameraPos, 1), mousePosWorld);
		
		// Make a plane through the body
		DualQuat lineHeadToNeck = join(Vec4(joints[0]->pos, 1), Vec4(joints[1]->pos, 1));

		// Join this with the shoulder to make the plane
		Plane plane = join(
			lineHeadToNeck,
			Vec4(joints[3]->pos, 1)
		);

		Vec4 pos = meet(line, plane);
		target->pos = VMath::perspectiveDivide(pos);

		break;
	}
}

void Scene4p::Update(const float deltaTime) {
	// Let's set up an orbit camera using the trackball
	// Rotate the position
	// Based on Umer's scribbles, we just need to divide the quaternions
	Quaternion qInitial = cameraOrientation;
	Quaternion qFinal = trackball.getQuat();
	// Fancy word to inverse a quaternion is "conjugate"
	Quaternion changeInRotation = qFinal * QMath::conjugate(qInitial); // qFinal / qInitial;
	cameraOrientation = trackball.getQuat();

	// We want to rotate around the head of the character
	// Following Umer's scribbles we need three steps
	// STEP 1: Translate so that the ball is at the origin
	cameraPos -= joints[0]->pos;
	// STEP 2: Rotate now
	cameraPos = QMath::rotate(cameraPos, changeInRotation);
	// STEP 3: Translate back
	cameraPos += joints[0]->pos;

	Matrix4 T = MMath::translate(cameraPos);         // T for Translation Matrix
	Matrix4 R = MMath::toMatrix4(cameraOrientation); // R for Rotation Matrix

	viewMatrix = MMath::inverse(R) * MMath::inverse(T);
}

void Scene4p::Render() const {
	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
	
	Vec4 green = Vec3(0, 1, 0);
	Vec4 red = Vec3(1, 0, 0);
	Vec4 blue = Vec3(0, 0, 1);
	Vec4 pink = Vec3(224, 99, 208) / 255;
	glUniform4fv(shader->GetUniformID("color"), 1, Vec4(pink, 0));
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, target->GetModelMatrix());
	// Let's just use the head's mesh as it's a sphere anyway
	joints[0]->GetMesh()->Render(GL_TRIANGLES);

	// Umer likes the jellyfish from Spongebob
	Vec4 color = blue; // a darker pink for the anchors
	int loops = 0;
	for (Body* joint : joints) {
		if (loops > 2 && loops < 6) {
			color = red;
		}
		else if (loops > 5 && loops < 10) {
			color = green;
		}
		else if (loops > 8 && loops < 13) {
			color = red;
		}
		else if (loops > 11) {
			color = green;
		}
		else {
			color = blue;
		}
		//std::cout << loops << std::endl;
		glUniform4fv(shader->GetUniformID("color"), 1, Vec4(color, 0));

		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, joint->GetModelMatrix());
		// Let's just use the head's mesh as it's a sphere anyway
		joints[0]->GetMesh()->Render(GL_TRIANGLES);
		loops++;
	}

	glUseProgram(0);
}
