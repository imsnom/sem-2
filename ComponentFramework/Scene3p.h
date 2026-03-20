// Scene3P for 3rd Physics assignment
#ifndef SCENE3P_H
#define SCENE3P_H
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

class Scene3p : public Scene {
private:
	// We will have one big sphere for the jellyfish's head 
	Body* jellyfishHead;
	// Smaller spheres just to show us where the tentacles are anchored 
	std::vector<Body*> anchors;
	// And the smallest spheres will make up the tentacles 
	std::vector<Body*> tentacleSpheres;
	// These variables will tells us how long each tentacle will be  
	// and the spacing between individual spheres 
	int numSpheresPerAnchor;
	float spacing;

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
	explicit Scene3p();
	virtual ~Scene3p();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SCENE3P_H