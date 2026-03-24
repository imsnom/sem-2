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
	std::vector<Matrix4> modelMatricesAnchors;
	std::vector<Matrix4> modelMatricesTentacleSpheres;
	std::vector<Body*> tentacleSpheres1;
	std::vector<Body*> tentacleSpheres2;
	std::vector<Body*> tentacleSpheres3;
	std::vector<Body*> tentacleSpheres4;
	std::vector<Body*> tentacleSpheres5;
	std::vector<Body*> tentacleSpheres6;
	std::vector<Body*> tentacleSpheres7;
	std::vector<Body*> tentacleSpheres8;
	std::vector<Body*> tentacleSpheres9;
	std::vector<Body*> tentacleSpheres10;
	std::vector<std::vector<Body*>> tSpheres = { tentacleSpheres1, tentacleSpheres2, tentacleSpheres3,
	tentacleSpheres4, tentacleSpheres5, tentacleSpheres6, tentacleSpheres7, tentacleSpheres8, tentacleSpheres9, tentacleSpheres10};
	// And the smallest spheres will make up the tentacles 
	



	// These variables will tells us how long each tentacle will be  
	// and the spacing between individual spheres 
	const int numSpheresPerAnchor;
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