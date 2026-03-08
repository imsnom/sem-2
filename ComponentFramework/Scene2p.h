#ifndef SCENE2P_H
#define SCENE2P_H
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
#include <Quaternion.h>
#include "Trackball.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;
class Shader;

class Scene2p : public Scene {
private:
	Body* cueBall;
	Body* targetBall;
	Body* plane;
	Shader* shader;
	Mesh* mesh;
	Mesh* sphereMesh;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 modelMatrix;
	Matrix4 rotation;
	Vec3 planeNormal;
	Vec3 axis;
	bool drawInWireMode;
	Trackball trackball;


	Vec3 cameraPos;
	Quaternion cameraOrientation;

public:
	explicit Scene2p();
	virtual ~Scene2p();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SCENE2P_H