#ifndef SCENE3_H
#define SCENE3_H
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
#include "Trackball.h"
#include "Camera.h"
#include "Skybox.h"
#include <vector>
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;
class Shader;
class Texture;

class Scene3g : public Scene {
private:
	Matrix4 sphereModelMatrix;
	bool drawInWireMode;
	Vec3 lightPos;
	Shader* shader;
	Camera camera;
	Body* sphere;
	Mesh* sphereMesh;
	Texture* sphereTexture;
	Skybox* skybox;


public:
	explicit Scene3g();
	virtual ~Scene3g();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SCENE3_H