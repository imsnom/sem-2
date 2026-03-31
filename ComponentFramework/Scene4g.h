//#ifndef SCENE4_H
//#define SCENE4_H
//#include "Scene.h"
//#include "Vector.h"
//#include <Matrix.h>
//#include "Trackball.h"
//#include "Camera.h"
//using namespace MATH;
//
///// Forward declarations 
//union SDL_Event;
//class Body;
//class Mesh;
//class Shader;
//class Texture;
//
//class Scene4g : public Scene {
//private:
//	Matrix4 sphereModelMatrix;
//	bool drawInWireMode;
//	Vec3 lightPos;
//	Shader* shader;
//	Camera camera;
//
//public:
//	explicit Scene4g();
//	virtual ~Scene4g();
//
//	virtual bool OnCreate() override;
//	virtual void OnDestroy() override;
//	virtual void Update(const float deltaTime) override;
//	virtual void Render() const override;
//	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
//};
//
//
//#endif // SCENE3_H