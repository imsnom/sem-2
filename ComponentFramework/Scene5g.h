#ifndef SCENE5G_H
#define SCENE5G_H
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
#include "Trackball.h"
#include "Camera.h"
#include "Texture.h"
#include "cmath"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;
class Shader;
class Texture;

class Scene5g : public Scene {
private:
	/*Matrix5 sphereModelMatrix;
	bool drawInWireMode;
	Vec3 lightPos;
	Shader* shader;*/

	Camera camera;

	Body* plane;
	Shader* shader;
	Mesh* mesh;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 modelMatrix;
	bool drawInWireMode;
	float tessLevel;
	float densityLevel;
	Texture* terrainHeight;
	Texture* terrainNormal;
	Texture* terrainDiffuse;

public:
	explicit Scene5g();
	virtual ~Scene5g();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SCENE5G_H