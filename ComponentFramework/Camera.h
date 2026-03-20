#pragma once

#include "MMath.h"
#include "QMath.h"
#include "Skybox.h"
#include "Trackball.h"

using namespace MATH;
class Camera {
private:
	Matrix4 projection;
	Vec3 translation;
	Quaternion orientation;
	Skybox* skybox;
	bool useSkybox;
	Trackball trackball;
public:
	Camera();
	~Camera();
	Matrix4 getProjection() const { return projection; }
	Matrix4 getView() const {
		return MMath::translate(translation) * MMath::toMatrix4(orientation);
	}
	void Render() const;
	void HandleEvents(const SDL_Event& sdlEvent);



};

