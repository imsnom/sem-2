#ifndef SCENE3_H
#define SCENE3_H
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
#include "Texture.h"
#include "Trackball.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;
class Shader;

class Scene3g : public Scene {
private:
	//Body* sphere;
	Shader* shader;
	Mesh* mesh;
	Mesh* skullMesh;
	//Mesh* marioMesh;
	//Texture* earthTexture;
	//Texture* moonTexture;
	Texture* skullTexture;
	Matrix4 projection;
	Matrix4 view;
	Matrix4 skullModelMatrix, eyeBallModelMatrix, eyeBallModelMatrix_2;
	bool drawInWireMode;
	Vec3 lightPos;
	//Vec3 lightPos_2;
	//Vec3 lightPosMulti[2] = { lightPos, lightPos_2 };
	//Vec4 colour;
	Mesh* eyeBall;
	Mesh* eyeBall_2;
	Texture* eyeBallTexture;
	Trackball trackball;
	Matrix4 rotation;

public:
	explicit Scene3g();
	virtual ~Scene3g();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
};


#endif // SCENE3_H