// Scene4p for 3rd Physics assignment
#ifndef SCENE4P_H
#define SCENE4P_H
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
#include <Quaternion.h>
#include "Trackball.h"
#include <vector>
using namespace MATH;	

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;
class Shader;

class Scene4p : public Scene {
private:
	
	std::vector<Body*> joints;

	float lowerArmLengthLeft;
	float upperArmLengthLeft;
	float thighLength;
	float calfLength;
	float toeLength;
	char limb;


	Body* target;
	Shader* shader;
	// Colin is right, the body has a mesh in there
	// so we don't need to make another mesh for the sphere
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;


	// Don't tell Scott, but lets make a camera
	Vec3 cameraPos;
	Quaternion cameraOrientation;
	Trackball trackball;

public:
	explicit Scene4p();
	virtual ~Scene4p();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SCENE4P_H