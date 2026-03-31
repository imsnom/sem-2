//#ifndef SCENE1_H
//#define SCENE1_H
//#include "Scene.h"
//#include "Vector.h"
//#include <Matrix.h>
//#include "Texture.h"
//using namespace MATH;
//
///// Forward declarations 
//union SDL_Event;
//class Body;
//class Mesh;
//class Shader;
//
//class Scene1g : public Scene {
//private:
//	Body* sphere;
//	Shader* shader;
//	Mesh* mesh;
//	Mesh* skullMesh;
//	//Mesh* marioMesh;
//	Texture* earthTexture;
//	Texture* moonTexture;
//	Texture* skullTexture;
//	Matrix4 projectionMatrix;
//	Matrix4 viewMatrix;
//	Matrix4 earthModelMatrix, moonModelMatrix, skullModelMatrix;
//	bool drawInWireMode;
//	Vec3 lightPos;
//	Vec3 lightPos_2;
//	Vec3 lightPosMulti[2] = { lightPos, lightPos_2 };
//	Vec4 colour;
//
//public:
//	explicit Scene1g();
//	virtual ~Scene1g();
//
//	virtual bool OnCreate() override;
//	virtual void OnDestroy() override;
//	virtual void Update(const float deltaTime) override;
//	virtual void Render() const override;
//	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
//};
//
//
//#endif // SCENE0_H