#ifndef SCENE0PR_H
#define SCENE0PR_H
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
#include <Quaternion.h>
#include "Trackball.h"
#include "Texture.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;
class Shader;

class Scene0pr : public Scene {
private:
	Texture* earthTexture;
	Texture* eyeTexture;
	Texture* red;
	Body* cueBall;
	Body* targetBall;
	Body* goalBall;
	Body* plane;
	Shader* shader;
	Mesh* mesh;
	Mesh* sphereMesh;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 modelMatrix, eyeModelMatrix;
	Matrix4 rotation;
	Vec3 planeNormal;
	Vec3 axis;
	bool drawInWireMode;
	Trackball trackball;


	Vec3 cameraPos;
	Quaternion cameraOrientation;

public:
	explicit Scene0pr();
	virtual ~Scene0pr();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // Scene0pr_H